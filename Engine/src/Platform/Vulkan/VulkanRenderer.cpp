#include "anepch.h"
#include "VulkanRenderer.h"

#include <SDL.h>
#include <SDL_vulkan.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_vulkan.h>
#include <VkBootstrap.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <filesystem>
#include <slang.h>
#include <slang-com-ptr.h>

#include "ANE/Core/Application.h"
#include "ANE/Math/FMath.h"
#include "ANE/Utilities/API.h"
#include "..\..\ANE\Utilities\AneImGui.h"

using Slang::ComPtr;

#include "MeshLoader.h"
#include "VulkanInitializers.h"
#include "VulkanUtils.h"
#include "ANE/Core/Window.h"
#include "ANE/Math/Types/Matrix4x4.h"
#include "ANE/Math/Types/Quaternion.h"
#include "ANE/Renderer/Draw.h"
#include "ANE/Renderer/Mesh.h"
#include "ANE/Utilities/ImGuiUtilities.h"
#include "VulkanDescriptorLayoutBuilder.h"

namespace Vulkan
{
    VulkanRenderer::VulkanRenderer(SDL_Window* window)
    {
        _window = window;
    }

    void VulkanRenderer::Setup()
    {
        ANE_PROFILE_FUNCTION();
        if (_initialized)
        {
            ANE_ELOG_WARN("Unable to setup VulkanRenderer as it has already been initialized.");
            return;
        }

        SetupVulkan(_window);
        SetupImGui(_window);

        _initialized = true;
    }

    void VulkanRenderer::Render(const WindowProperties& props, const DrawContext& drawCommands)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if (!_initialized)
        {
            ANE_ELOG_WARN("Unable to end VulkanRenderer frame as it was never fully initialized.");
            return;
        }

        //ImGui::Render();
        const ImDrawData* drawData = ImGui::GetDrawData();
        const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
        if (!isMinimized)
        {
            _mainWindowData.ClearValue.color.float32[0] = ClearColor.x * ClearColor.w;
            _mainWindowData.ClearValue.color.float32[1] = ClearColor.y * ClearColor.w;
            _mainWindowData.ClearValue.color.float32[2] = ClearColor.z * ClearColor.w;
            _mainWindowData.ClearValue.color.float32[3] = ClearColor.w;
            Draw(props, drawCommands);
            //RenderFrame(&_mainWindowData, drawData);
        }

        // Update and Render additional Platform Windows
        //if (_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        //{
        //    ImGui::UpdatePlatformWindows();
        //    ImGui::RenderPlatformWindowsDefault();
        //}

        //if (!isMinimized)
        //{
        //    RevealFrame(&_mainWindowData);
        //}
    }

    void VulkanRenderer::Cleanup()
    {
        if (!_initialized)
        {
            ANE_ELOG_WARN("Unable to clean up VulkanRenderer as it was never fully initialized.");
            return;
        }

        CHECK_RESULT(vkDeviceWaitIdle(_device));

        CleanupImGui();
        CleanupVulkan();
        _initialized = false;
    }

    void VulkanRenderer::OnWindowResize()
    {
        ResizeMainBuffers();
        _rebuildSwapchain = true;
    }

    VmaMeshAsset* VulkanRenderer::LoadModel(const std::string& modelPath)
    {
        if (_loadedModelMap.contains(modelPath))
        {
            return _loadedModelMap[modelPath];
        }

        const std::string relativePath = std::string(ASSET_PATH_MESHES).append(modelPath);

        const MeshAsset* meshAsset = MeshLoader::LoadMesh(relativePath.c_str());
        if (meshAsset == nullptr)
        {
            //ANE_ELOG_WARN("Unable to load mesh");
            return nullptr;
        }
        Mesh mesh = meshAsset->SubMeshes[0]; // Use first submesh for now.

        const VmaMeshBuffers meshBuffers = UploadMesh(mesh.Indices, mesh.Vertices);

        VmaMeshAsset* vmaMeshAsset = new VmaMeshAsset();
        vmaMeshAsset->Name = modelPath;
        vmaMeshAsset->NumVertices = (uint32_t)mesh.Indices.size();
        vmaMeshAsset->SubMeshes = meshAsset->SubMeshes;
        vmaMeshAsset->MeshBuffers = meshBuffers;

        _mainDeletionQueue.PushFunction([=]
        {
            const VmaMeshBuffers buffers = vmaMeshAsset->MeshBuffers;
            DestroyBuffer(buffers.IndexBuffer);
            DestroyBuffer(buffers.VertexBuffer);
        });

        _loadedModelMap[modelPath] = vmaMeshAsset;

        return vmaMeshAsset;
    }

    VmaImage VulkanRenderer::LoadTexture(const std::string& texturePath)
    {
        if (_loadedTextureMap.contains(texturePath))
        {
            return _loadedTextureMap[texturePath];
        }

        const std::string assetPath = std::string(ASSET_PATH_TEXTURES).append(texturePath);

        int width, height, channels;
        stbi_uc* pixels = stbi_load(assetPath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!pixels)
        {
            ANE_ELOG_ERROR("Unable to load texture at path: {}", assetPath);
            return _errorImage;
        }

        const VkExtent3D imageExtent = VkExtent3D{ (uint32_t)width, (uint32_t)height, 1 };
        const VmaImage imageBuffers = CreateImage(pixels, imageExtent, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_VIEW_TYPE_2D, true);

        stbi_image_free(pixels);

        _mainDeletionQueue.PushFunction([=]
        {
            DestroyImage(imageBuffers);
        });

        _loadedTextureMap[texturePath] = imageBuffers;

        return imageBuffers;
    }

    VmaImage VulkanRenderer::LoadCubeTexture(const std::string& texturePath)
    {
        if (_loadedTextureMap.contains(texturePath))
        {
            return _loadedTextureMap[texturePath];
        }

        const std::string assetPath = std::string(ASSET_PATH_TEXTURES).append(texturePath);

        int width, height, channels;
        stbi_uc* pixels = stbi_load(assetPath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!pixels)
        {
            ANE_ELOG_ERROR("Unable to load texture at path: {}", assetPath);
            return _errorImage;
        }

        //const VkExtent3D imageExtent = VkExtent3D{ (uint32_t)width / 4, (uint32_t)height / 3, 1 }; // Horizontal flag layout
        const VkExtent3D imageExtent = VkExtent3D{ (uint32_t)width, (uint32_t)height / 6, 1 }; // Vertical layout
        const VmaImage imageBuffers = CreateCubeImage(pixels, imageExtent, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_VIEW_TYPE_CUBE, true);

        stbi_image_free(pixels);

        _mainDeletionQueue.PushFunction([=]
        {
            DestroyImage(imageBuffers);
        });

        _loadedTextureMap[texturePath] = imageBuffers;

        return imageBuffers;
    }

    float VulkanRenderer::GetFramerate()
    {
        return _io->Framerate;
    }

    MaterialInstance* VulkanRenderer::GetDefaultMaterialClone()
    {
        VmaBuffer materialConstants = CreateBuffer(sizeof(FilamentMetallicRoughness::MaterialConstants), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

        // Use uniforms from original material.
        auto* uniforms = (FilamentMetallicRoughness::MaterialConstants*)materialConstants.Allocation->GetMappedData();
        *uniforms = *_filamentInstance.Uniforms;

        auto resources = _filamentInstance.Resources;
        resources.Data = materialConstants;
        resources.DataBufferOffset = 0;

        _mainDeletionQueue.PushFunction([=]
        {
            DestroyBuffer(materialConstants);
        });

        MaterialInstance* material = new MaterialInstance();
        *material = _filamentMaterial.WriteMaterial(this, MaterialPass::Opaque, resources, uniforms, _mainDescriptors);
        _materialInstances.push_back(material);
        return _materialInstances.back();
    }

    void VulkanRenderer::SetupVulkan(SDL_Window* window)
    {
        const std::vector<const char*> extensions = GetAvailableExtensions(window);
        const vkb::Instance vkbInstance = CreateVkbInstance(extensions);
        _instance = vkbInstance.instance;
        _debugMessenger = vkbInstance.debug_messenger;

        if (SDL_Vulkan_CreateSurface(window, _instance, &_surface) == 0)
        {
            ANE_ELOG_ERROR("Could not create Vulkan surface.\n");
        }

        const vkb::PhysicalDevice physicalDevice = SelectVkbPhysicalDevice(_surface, vkbInstance);
        _physicalDevice = physicalDevice.physical_device;

        const vkb::Device logicalDevice = CreateVkbLogicalDevice(physicalDevice);
        _device = logicalDevice.device;

        vkb::Result<VkQueue> queueResult = logicalDevice.get_queue(vkb::QueueType::graphics);

        ANE_EASSERT(queueResult.has_value(), "Queue has no graphics support.");

        _queue = queueResult.value();

        CreateVmaAllocator();

        SetupSwapchain();

        SetupCommandBuffers();

        SetupSyncStructures();

        SetupDescriptors();

        CreatePipeline(logicalDevice);

        CreateDefaultResources();
    }

    void VulkanRenderer::SetupImGui(SDL_Window* window)
    {
        IMGUI_CHECKVERSION();
        CreateImGuiDescriptorPool();
        AneImGui::Initialize();

        _io = &ImGui::GetIO(); (void)_io;
        _io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        _io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        _io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        //_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows. Doesn't seem to like custom swapchains.
        //_io->ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        //_io->ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
        _io->ConfigWindowsMoveFromTitleBarOnly = true;

        // Setup Dear ImGui style
        ImGuiUtilities::StyleAnemoneDark();
        //ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForVulkan(window);
        const ImGui_ImplVulkanH_Window* wd = &_mainWindowData;

        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = _instance;
        initInfo.PhysicalDevice = _physicalDevice;
        initInfo.Device = _device;
        initInfo.QueueFamily = _queueFamily.GraphicsFamily.value();
        initInfo.Queue = _queue;
        initInfo.PipelineCache = _pipelineCache;
        initInfo.DescriptorPool = _imGuiDescriptorPool;
        initInfo.RenderPass = wd->RenderPass;
        initInfo.Subpass = 0;
        initInfo.MinImageCount = _minImageCount;
        initInfo.ImageCount = _minImageCount;
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        initInfo.UseDynamicRendering = true;
        VkPipelineRenderingCreateInfoKHR renderingInfo{ .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR, .pNext = nullptr };
        renderingInfo.colorAttachmentCount = 1;
        const VkFormat colorFormat = { _swapchainImageFormat };
        renderingInfo.pColorAttachmentFormats = &colorFormat;
        initInfo.PipelineRenderingCreateInfo = renderingInfo;

        initInfo.Allocator = _allocator;
        initInfo.CheckVkResultFn = CheckVkResult;

        ImGui_ImplVulkan_Init(&initInfo);

        UpdateImGuiViewportSet();
    }

    std::vector<const char*> VulkanRenderer::GetAvailableExtensions(SDL_Window* window)
    {
        uint32_t extensionCount = 0;
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
        std::vector<const char*> extensions(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data());

        if (enableValidationLayers)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    vkb::Instance VulkanRenderer::CreateVkbInstance(const std::vector<const char*>& extensions)
    {
        constexpr VkDebugUtilsMessageSeverityFlagBitsEXT debugMessageSeverityFlags = (VkDebugUtilsMessageSeverityFlagBitsEXT)(
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
        constexpr VkDebugUtilsMessageTypeFlagBitsEXT debugMessageTypeFlags = (VkDebugUtilsMessageTypeFlagBitsEXT)(
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT);

        vkb::InstanceBuilder builder;

        vkb::Result<vkb::Instance> vkbResult = builder
            .set_engine_name("NP")
            .set_app_name("Sandbox")
            .require_api_version(1, 3, 0)

            .enable_extensions(extensions)

            .request_validation_layers(enableValidationLayers)
            .set_debug_messenger_severity(debugMessageSeverityFlags)
            .set_debug_messenger_type(debugMessageTypeFlags)
            .set_debug_callback(DebugCallback)

            .set_allocation_callbacks(_allocator)
            .build();

        return vkbResult.value();
    }

    vkb::PhysicalDevice VulkanRenderer::SelectVkbPhysicalDevice(VkSurfaceKHR surface, vkb::Instance instance)
    {
        // Vulkan 1.3 features.
        VkPhysicalDeviceVulkan13Features features = {};
        features.dynamicRendering = true;
        features.synchronization2 = true;

        // Vulkan 1.2 features.
        VkPhysicalDeviceVulkan12Features features12 = {};
        features12.bufferDeviceAddress = true;
        features12.descriptorIndexing = true;

        // Vulkan 1.1 features.
        VkPhysicalDeviceVulkan11Features features11 = {};
        features11.variablePointersStorageBuffer = VK_TRUE;
        features11.variablePointers = VK_TRUE;

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.fillModeNonSolid = VK_TRUE;
        deviceFeatures.shaderInt64 = VK_TRUE;

        // Use VkBootstrap to select a gpu.
        // We want a gpu that can write to the SDL surface and supports vulkan 1.3 with the correct features.
        vkb::PhysicalDeviceSelector selector{ instance };
        vkb::PhysicalDevice physicalDevice = selector
            .set_minimum_version(1, 3)

            .set_required_features(deviceFeatures)

            .set_required_features_13(features)
            .set_required_features_12(features12)
            .set_required_features_11(features11)

            .prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
            .add_required_extension("VK_KHR_swapchain")

            .set_surface(surface)
            .select()
            .value();

        return physicalDevice;
    }

    vkb::Device VulkanRenderer::CreateVkbLogicalDevice(const vkb::PhysicalDevice& physicalDevice)
    {
        std::vector<vkb::CustomQueueDescription> queueDescriptions;
        std::vector<VkQueueFamilyProperties> queueFamilies = physicalDevice.get_queue_families();
        QueueFamilyIndices indices;
        for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilies.size ()); i++) {
            if (!(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) continue;

            queueDescriptions.emplace_back(i, std::vector(queueFamilies[i].queueCount, 1.0f));
            indices.GraphicsFamily = i;
            break;
        }
        _queueFamily = indices;

        // Create the final vulkan device.
        vkb::DeviceBuilder deviceBuilder{ physicalDevice };

        vkb::Device logicalDevice = deviceBuilder
            .set_allocation_callbacks(_allocator)
            .custom_queue_setup(queueDescriptions)

            .build()
            .value();

        return logicalDevice;
    }

    void VulkanRenderer::SetupSwapchain()
    {
        const auto props = Application::Get().GetWindow().GetWindowProperties();
        _windowExtent.width = props.Width;
        _windowExtent.height = props.Height;

        vkb::Swapchain swapchain = CreateSwapchain(_windowExtent.width, _windowExtent.height);
        _swapchainExtent = swapchain.extent;
        _swapchain = swapchain.swapchain;
        _swapchainImages = swapchain.get_images().value();
        _swapchainImageViews = swapchain.get_image_views().value();

        const auto viewportProps = Application::Get().GetWindow().GetWindowProperties();

        CreateMainBuffers(viewportProps.Width, viewportProps.Height);
    }

    vkb::Swapchain VulkanRenderer::CreateSwapchain(const uint32_t width, const uint32_t height)
    {
        vkb::SwapchainBuilder builder{_physicalDevice, _device, _surface};

        _swapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

        const vkb::Swapchain swapchain = builder
             .set_desired_format(VkSurfaceFormatKHR{ .format = _swapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
             .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)

             .set_desired_extent(width, height)
             .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)

             .build()
             .value();

        return swapchain;
    }

    void VulkanRenderer::DestroySwapchain()
    {
        vkDestroySwapchainKHR(_device, _swapchain, _allocator);
        for (const VkImageView& swapchainImageView : _swapchainImageViews)
        {
            vkDestroyImageView(_device, swapchainImageView, _allocator);
        }
    }

    void VulkanRenderer::ResizeSwapchain()
    {
        vkDeviceWaitIdle(_device);

        DestroySwapchain();

        const auto props = Application::Get().GetWindow().GetWindowProperties();
        _windowExtent.width = props.Width;
        _windowExtent.height = props.Height;

        vkb::Swapchain swapchain = CreateSwapchain(_windowExtent.width, _windowExtent.height);
        _swapchainExtent = swapchain.extent;
        _swapchain = swapchain.swapchain;
        _swapchainImages = swapchain.get_images().value();
        _swapchainImageViews = swapchain.get_image_views().value();

        _rebuildSwapchain = false;
    }

    void VulkanRenderer::CreateMainBuffers(const uint32_t width, const uint32_t height)
    {
        const VkExtent3D imageExtent = { width, height, 1 };

        // Setup color buffer.
        _colorImage.ImageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
        _colorImage.ImageExtent = imageExtent;

        VkImageUsageFlags drawImageUsages{};
        drawImageUsages |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        drawImageUsages |= VK_IMAGE_USAGE_STORAGE_BIT;
        drawImageUsages |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        drawImageUsages |= VK_IMAGE_USAGE_SAMPLED_BIT;

        const VkImageCreateInfo cImgInfo = VulkanInitializers::ImageCreateInfo(_colorImage.ImageFormat, drawImageUsages, imageExtent);

        VmaAllocationCreateInfo cImgAllocInfo = { };
        cImgAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        cImgAllocInfo.requiredFlags = (VkMemoryPropertyFlags)VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        CHECK_RESULT(vmaCreateImage(_vmaAllocator, &cImgInfo, &cImgAllocInfo, &_colorImage.Image, &_colorImage.Allocation, nullptr));

        const VkImageViewCreateInfo cViewInfo = VulkanInitializers::ImageViewCreateInfo(_colorImage.ImageFormat, _colorImage.Image, VK_IMAGE_ASPECT_COLOR_BIT);

        CHECK_RESULT(vkCreateImageView(_device, &cViewInfo, _allocator, &_colorImage.ImageView));

        // Setup depth buffer.
        _depthImage.ImageFormat = VK_FORMAT_D32_SFLOAT;
        _depthImage.ImageExtent = imageExtent;

        VkImageUsageFlags depthImageUsages{};
        depthImageUsages |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        const VkImageCreateInfo dImgInfo = VulkanInitializers::ImageCreateInfo(_depthImage.ImageFormat, depthImageUsages, imageExtent);

        CHECK_RESULT(vmaCreateImage(_vmaAllocator, &dImgInfo, &cImgAllocInfo, &_depthImage.Image, &_depthImage.Allocation, nullptr));

        const VkImageViewCreateInfo dViewInfo = VulkanInitializers::ImageViewCreateInfo(_depthImage.ImageFormat, _depthImage.Image, VK_IMAGE_ASPECT_DEPTH_BIT);

        CHECK_RESULT(vkCreateImageView(_device, &dViewInfo, _allocator, &_depthImage.ImageView));
    }

    void VulkanRenderer::DestroyMainBuffers()
    {
        vkDestroyImageView(_device, _colorImage.ImageView, _allocator);
        vmaDestroyImage(_vmaAllocator, _colorImage.Image, _colorImage.Allocation);

        vkDestroyImageView(_device, _depthImage.ImageView, _allocator);
        vmaDestroyImage(_vmaAllocator, _depthImage.Image, _depthImage.Allocation);
    }

    void VulkanRenderer::ResizeMainBuffers()
    {
        const auto props = Application::Get().GetWindow().GetActiveViewportProperties();
        //_windowExtent.width = props.Width;
        //_windowExtent.height = props.Height;

        ResizeMainBuffers(props.Width, props.Height);
    }

    void VulkanRenderer::ResizeMainBuffers(const uint32_t width, const uint32_t height)
    {
        vkDeviceWaitIdle(_device);

        DestroyMainBuffers();

        CreateMainBuffers(width, height);

        UpdateImGuiViewportSet();
    }

    void VulkanRenderer::UpdateImGuiViewportSet()
    {
        // image_layout should be what the VkImageView was before drawing ImGui. (See: Draw())
        _imGuiViewportSet = ImGui_ImplVulkan_AddTexture(_samplerNearest, _colorImage.ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }

    void VulkanRenderer::SetupCommandBuffers()
    {
        const uint32_t familyIndex = _queueFamily.GraphicsFamily.value();
        const VkCommandPoolCreateInfo commandPoolInfo = VulkanInitializers::CommandPoolCreateInfo(familyIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

        // Frame buffers.
        for (VulkanFrame& frame : _frameData)
        {
            CHECK_RESULT(vkCreateCommandPool(_device, &commandPoolInfo, _allocator, &frame.CommandPool));

            VkCommandBufferAllocateInfo cmdAllocInfo = VulkanInitializers::CommandBufferAllocateInfo(frame.CommandPool, 1);

            CHECK_RESULT(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &frame.CommandBuffer));

            _mainDeletionQueue.PushFunction([=]
            {
                vkDestroyCommandPool(_device, frame.CommandPool, _allocator);
            });
        }

        // Immediate buffer.
        CHECK_RESULT(vkCreateCommandPool(_device, &commandPoolInfo, _allocator, &_immBuffer.CommandPool));

        const VkCommandBufferAllocateInfo cmdAllocInfo = VulkanInitializers::CommandBufferAllocateInfo(_immBuffer.CommandPool, 1);

        CHECK_RESULT(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &_immBuffer.CommandBuffer));

        _mainDeletionQueue.PushFunction([=]
        {
            vkDestroyCommandPool(_device, _immBuffer.CommandPool, _allocator);
        });
    }

    void VulkanRenderer::SetupSyncStructures()
    {
        const VkFenceCreateInfo fenceCreateInfo = VulkanInitializers::FenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
        CHECK_RESULT(vkCreateFence(_device, &fenceCreateInfo, _allocator, &_immBuffer.Fence));

        _mainDeletionQueue.PushFunction([=]
        {
            vkDestroyFence(_device, _immBuffer.Fence, _allocator);
        });

        for (VulkanFrame& frame : _frameData)
        {
            CHECK_RESULT(vkCreateFence(_device, &fenceCreateInfo, _allocator, &frame.Fence));

            VkSemaphoreCreateInfo semaphoreCreateInfo = VulkanInitializers::SemaphoreCreateInfo();

            CHECK_RESULT(vkCreateSemaphore(_device, &semaphoreCreateInfo, _allocator, &frame.SwapchainSemaphore));
            CHECK_RESULT(vkCreateSemaphore(_device, &semaphoreCreateInfo, _allocator, &frame.RenderSemaphore));

            _mainDeletionQueue.PushFunction([=]
            {
                vkDestroyFence(_device, frame.Fence, _allocator);
                vkDestroySemaphore(_device, frame.SwapchainSemaphore, _allocator);
                vkDestroySemaphore(_device, frame.RenderSemaphore, _allocator);
            });
        }
    }

    void VulkanRenderer::SetupDescriptors()
    {
        std::vector<DescriptorAllocator::PoolSizeRatio> sizes =
        {
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 3 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3 },
        };

        _mainDescriptors.Init(_device, 10, sizes, _allocator);
        _mainDeletionQueue.PushFunction([&]
        {
            _mainDescriptors.Destroy(_device, _allocator);
        });

        {
            DescriptorLayoutBuilder builder { _device };
            _appDataLayout = builder
                .SetStageFlags(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
                .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)

                .SetAllocationCallbacks(_allocator)
                .Build();

            _geometryDataLayout = builder.Build();

            _mainDeletionQueue.PushFunction([&]
            {
                vkDestroyDescriptorSetLayout(_device, _appDataLayout, _allocator);
                vkDestroyDescriptorSetLayout(_device, _geometryDataLayout, _allocator);
            });
        }

        {
            DescriptorLayoutBuilder builder { _device };
            _singleImageDataLayout = builder
                .SetStageFlags(VK_SHADER_STAGE_FRAGMENT_BIT)
                .AddBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)

                .SetAllocationCallbacks(_allocator)
                .Build();

            _mainDeletionQueue.PushFunction([&]
            {
                vkDestroyDescriptorSetLayout(_device, _singleImageDataLayout, _allocator);
            });
        }

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            std::vector<DescriptorAllocator::PoolSizeRatio> frameSizes =
            {
                { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 3 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3 },
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4 },
            };

            _frameData[i].Descriptors = DescriptorAllocator{};
            _frameData[i].Descriptors.Init(_device, 1000, frameSizes, _allocator);
            _frameData[i].AppDataBuffer = CreateBuffer(sizeof(ApplicationData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
            _frameData[i].SceneDataBuffer = CreateBuffer(sizeof(SceneData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

            _mainDeletionQueue.PushFunction([&, i]
            {
                DestroyBuffer(_frameData[i].AppDataBuffer);
                DestroyBuffer(_frameData[i].SceneDataBuffer);
                _frameData[i].Descriptors.Destroy(_device, _allocator);
            });
        }
    }

    void VulkanRenderer::LoadSlangShader(const char* moduleName, VkShaderModule* vertShader, VkShaderModule* fragShader)
    {
        ComPtr<slang::IGlobalSession> slangGlobalSession;
        createGlobalSession(slangGlobalSession.writeRef());

        slang::TargetDesc targetDesc = { };
        targetDesc.format = SLANG_SPIRV;
        targetDesc.profile = slangGlobalSession->findProfile("glsl_440");
        targetDesc.flags = SLANG_TARGET_FLAG_GENERATE_SPIRV_DIRECTLY;

        slang::SessionDesc sessionDesc = { };
        sessionDesc.targets = &targetDesc;
        sessionDesc.targetCount = 1;
        sessionDesc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR;
        const char* paths[] = { ASSET_PATH_SHADERS };
        sessionDesc.searchPaths = paths;
        sessionDesc.searchPathCount = 1;

        ComPtr<slang::ISession> session;
        slangGlobalSession->createSession(sessionDesc, session.writeRef());

        ComPtr<slang::IBlob> spirvVertProgram;
        ComPtr<slang::IBlob> spirvFragProgram;
        ComPtr<slang::IBlob> diagnosticBlob;
        slang::IModule* slangModule = session->loadModule(moduleName, diagnosticBlob.writeRef());
        //diagnoseIfNeeded(diagnosticBlob);

        if (!slangModule)
        {
            ANE_ELOG_ERROR("Error when loading shader module: {}", moduleName);
        }

        ComPtr<slang::IEntryPoint> vertEntry;
        slangModule->findEntryPointByName("vertexMain", vertEntry.writeRef());

        slang::IComponentType* components[] = { slangModule, vertEntry };
        ComPtr<slang::IComponentType> program;
        {
            ComPtr<slang::IBlob> diagnosticsBlob;
            SlangResult result = session->createCompositeComponentType(
                components,
                2,
                program.writeRef(),
                diagnosticsBlob.writeRef());
            // diagnoseIfNeeded(diagnosticsBlob);
            // RETURN_ON_FAIL(result);
        }

        {
            ComPtr<slang::IBlob> diagnosticsBlob;
            SlangResult result = program->getEntryPointCode(
                0, 0, spirvVertProgram.writeRef(), diagnosticsBlob.writeRef());
            //diagnoseIfNeeded(diagnosticsBlob);
            //RETURN_ON_FAIL(result);
        }

        ComPtr<slang::IEntryPoint> fragEntry;
        slangModule->findEntryPointByName("fragmentMain", fragEntry.writeRef());

        slang::IComponentType* components2[] = { slangModule, fragEntry };
        ComPtr<slang::IComponentType> program2;
        {
            ComPtr<slang::IBlob> diagnosticsBlob;
            SlangResult result = session->createCompositeComponentType(
                components2,
                2,
                program2.writeRef(),
                diagnosticsBlob.writeRef());
            // diagnoseIfNeeded(diagnosticsBlob);
            // RETURN_ON_FAIL(result);
        }

        {
            ComPtr<slang::IBlob> diagnosticsBlob;
            SlangResult result = program2->getEntryPointCode(
                0, 0, spirvFragProgram.writeRef(), diagnosticsBlob.writeRef());
            //diagnoseIfNeeded(diagnosticsBlob);
            //RETURN_ON_FAIL(result);
        }

        if (!VulkanUtils::LoadShaderModule(spirvVertProgram, _device, _allocator, vertShader))
        {
            ANE_ELOG_ERROR("Error when building vertex shader module: {}", slangModule->getName());
        }

        if (!VulkanUtils::LoadShaderModule(spirvFragProgram, _device, _allocator, fragShader))
        {
            ANE_ELOG_ERROR("Error when building fragment shader module: {}", slangModule->getName());
        }
    }

    void VulkanRenderer::CreatePipeline(const vkb::Device& logicalDevice)
    {
        VkPushConstantRange bufferRange;
        bufferRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        bufferRange.offset = 0;
        bufferRange.size = sizeof(PushConstantBuffer);

        VkDescriptorSetLayout layouts[] = { _geometryDataLayout, _appDataLayout, _singleImageDataLayout };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = VulkanInitializers::PipelineLayoutCreateInfo();
        pipelineLayoutInfo.setLayoutCount = (uint32_t)IM_ARRAYSIZE(layouts);
        pipelineLayoutInfo.pSetLayouts = layouts;
        pipelineLayoutInfo.pPushConstantRanges = &bufferRange;
        pipelineLayoutInfo.pushConstantRangeCount = 1;

        CHECK_RESULT(vkCreatePipelineLayout(_device, &pipelineLayoutInfo, _allocator, &_pipelineLayout));

        VulkanPipelineBuilder builder{ logicalDevice, _pipelineLayout };

        VkShaderModule meshVertShader, meshFragShader;
        LoadSlangShader("Mesh_Sky", &meshVertShader, &meshFragShader);

        vkb::Result<PipelineWrapper> skyPipeline = builder
            .SetShaders(meshVertShader, meshFragShader)
            .SetBlendMode(None)
            .SetDepthTestOperator(VK_COMPARE_OP_ALWAYS) // Reverse Z for more precision
            .SetDepthWrite(false)
            .SetCullMode(VK_CULL_MODE_FRONT_BIT)

            .SetColorFormat(_colorImage.ImageFormat)
            .SetDepthFormat(_depthImage.ImageFormat)

            .SetAllocationCallbacks(_allocator)
            .Build();

        _skyPipeline = skyPipeline->Pipeline;

        // Cleanup.
        vkDestroyShaderModule(_device, meshFragShader, _allocator);
        vkDestroyShaderModule(_device, meshVertShader, _allocator);

        #ifndef ANE_DIST
        VkShaderModule debugVertShader, debugFragShader;
        LoadSlangShader("Mesh_Wireframe", &debugVertShader, &debugFragShader);

        vkb::Result<PipelineWrapper> debugTrianglePipeline = builder
            .SetShaders(debugVertShader, debugFragShader)
            .SetBlendMode(Alpha)
            .SetDepthTestOperator(VK_COMPARE_OP_ALWAYS) // Reverse Z for more precision
            .SetDepthWrite(false)
            .SetCullMode(VK_CULL_MODE_NONE)

            .SetColorFormat(_colorImage.ImageFormat)
            .SetDepthFormat(_depthImage.ImageFormat)
            .SetPolygonMode(VK_POLYGON_MODE_LINE)
            .SetTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)

            .SetAllocationCallbacks(_allocator)
            .Build();
        _debugTrianglePipeline = debugTrianglePipeline->Pipeline;

        vkb::Result<PipelineWrapper> debugLinePipeline = builder
            .SetShaders(debugVertShader, debugFragShader)
            .SetBlendMode(None) //Alpha doesnt work with lines?
            .SetDepthTestOperator(VK_COMPARE_OP_ALWAYS) // Reverse Z for more precision
            .SetDepthWrite(false)
            .SetCullMode(VK_CULL_MODE_NONE)

            .SetColorFormat(_colorImage.ImageFormat)
            .SetDepthFormat(_depthImage.ImageFormat)
            .SetPolygonMode(VK_POLYGON_MODE_LINE)
            .SetTopology(VK_PRIMITIVE_TOPOLOGY_LINE_LIST)

            .SetAllocationCallbacks(_allocator)
            .Build();
        _debugLinePipeline = debugLinePipeline->Pipeline;

        // Cleanup.
        vkDestroyShaderModule(_device, debugFragShader, _allocator);
        vkDestroyShaderModule(_device, debugVertShader, _allocator);
        #endif

        _mainDeletionQueue.PushFunction([&]
        {
            vkDestroyPipelineLayout(_device, _pipelineLayout, _allocator);
            vkDestroyPipeline(_device, _skyPipeline, _allocator);
            vkDestroyPipeline(_device, _debugTrianglePipeline, _allocator);
            vkDestroyPipeline(_device, _debugLinePipeline, _allocator);
        });
    }

    void VulkanRenderer::CreateDefaultResources()
    {
        CreateDefaultTextures();

        _colorTex = LoadTexture("MetalTiles03_1K_BaseColor.png");
        _normalTex = LoadTexture("MetalTiles03_1K_Normal.png");
        _ormTex = LoadTexture("MetalTiles03_1K_ORM.png");
        _dfgTex = LoadTexture("dfg.png");
        _cubeMap = LoadCubeTexture("cyclorama_hard_light_linear_256.png");

        _cubeMesh = LoadModel("Cube.obj");
        _sphereMesh = LoadModel("Sphere.fbx");
        _skyMesh = LoadModel("Sphere.fbx");

        CreateDefaultMaterial();
    }

    void VulkanRenderer::CreateDefaultTextures()
    {
        constexpr VkExtent3D defaultImageExtent = VkExtent3D{ 16, 16, 1 };
        constexpr VkFormat defaultImageFormat = VK_FORMAT_R8G8B8A8_UNORM;

        const uint32_t white = glm::packUnorm4x8(Vector4(1, 1, 1, 1));
        const uint32_t black = glm::packUnorm4x8(Vector4(0, 0, 0, 1));
        const uint32_t grey = glm::packUnorm4x8(Vector4(0.66f, 0.66f, 0.66f, 1));
        const uint32_t magenta = glm::packUnorm4x8(Vector4(1, 0, 1, 1));
        const uint32_t normal = glm::packUnorm4x8(Vector4(0.5f, 0.5f, 1, 1));
        uint32_t pixels[16 * 16];

        for (uint32_t& pixel : pixels) { pixel = white; }
        _whiteImage = CreateImage((void*)&pixels, defaultImageExtent, defaultImageFormat, VK_IMAGE_USAGE_SAMPLED_BIT);

        for (uint32_t& pixel : pixels) { pixel = black; }
        _blackImage = CreateImage((void*)&pixels, defaultImageExtent, defaultImageFormat, VK_IMAGE_USAGE_SAMPLED_BIT);

        for (uint32_t& pixel : pixels) { pixel = grey; }
        _greyImage = CreateImage((void*)&pixels, defaultImageExtent, defaultImageFormat, VK_IMAGE_USAGE_SAMPLED_BIT);

        for (uint32_t& pixel : pixels) { pixel = normal; }
        _normalImage = CreateImage((void*)&pixels, defaultImageExtent, defaultImageFormat, VK_IMAGE_USAGE_SAMPLED_BIT);

        for (int x = 0; x < 16; x++)
        {
            for (int y = 0; y < 16; y++)
            {
                pixels[y * 16 + x] = (x % 2) ^ (y % 2) ? magenta : black;
            }
        }
        _errorImage = CreateImage((void*)&pixels, defaultImageExtent, defaultImageFormat, VK_IMAGE_USAGE_SAMPLED_BIT);

        constexpr VkExtent3D defaultCubeImageExtent = VkExtent3D{ 16, 16, 1 };
        constexpr VkFormat defaultCubeImageFormat = VK_FORMAT_R8G8B8A8_UNORM;

        uint32_t pixelsCube[16 * 16 * 6];
        for (uint32_t& pixel : pixelsCube) { pixel = black; }
        _blackCubeImage = CreateCubeImage((void*)&pixelsCube, defaultCubeImageExtent, defaultCubeImageFormat, VK_IMAGE_USAGE_SAMPLED_BIT);

        VkSamplerCreateInfo samplerInfo = { .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, .pNext = nullptr };
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
        samplerInfo.mipLodBias = 0.0f;

        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        vkCreateSampler(_device, &samplerInfo, _allocator, &_samplerLinear);

        samplerInfo.magFilter = VK_FILTER_NEAREST;
        samplerInfo.minFilter = VK_FILTER_NEAREST;
        vkCreateSampler(_device, &samplerInfo, _allocator, &_samplerNearest);

        _mainDeletionQueue.PushFunction([=]
        {
            DestroyImage(_whiteImage);
            DestroyImage(_blackImage);
            DestroyImage(_greyImage);
            DestroyImage(_normalImage);
            DestroyImage(_errorImage);

            DestroyImage(_blackCubeImage);

            vkDestroySampler(_device, _samplerLinear, _allocator);
            vkDestroySampler(_device, _samplerNearest, _allocator);
        });
    }

    void VulkanRenderer::CreateDefaultMaterial()
    {
        // Filament material instance.
        _filamentMaterial.BuildPipelines(this);
        _mainDeletionQueue.PushFunction([&]
        {
            _filamentMaterial.ClearResources(this);
        });

        FilamentMetallicRoughness::MaterialResources materialResources;
        materialResources.ColorImage = _colorTex;
        materialResources.ColorSampler = _samplerLinear;
        materialResources.NormalImage = _normalTex;
        materialResources.NormalSampler = _samplerLinear;
        materialResources.ORMImage = _ormTex;
        materialResources.ORMSampler = _samplerLinear;
        materialResources.DFGImage = _dfgTex;
        materialResources.DFGSampler = _samplerLinear;
        materialResources.CubeMap = _cubeMap;
        materialResources.CubeMapSampler = _samplerLinear;

        VmaBuffer materialConstants = CreateBuffer(sizeof(FilamentMetallicRoughness::MaterialConstants), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

        auto* sceneUniformData = (FilamentMetallicRoughness::MaterialConstants*)materialConstants.Allocation->GetMappedData();
        *sceneUniformData = { }; // Initialize using defaults values defined in the struct.

        materialResources.Data = materialConstants;
        materialResources.DataBufferOffset = 0;

        _filamentInstance = _filamentMaterial.WriteMaterial(this, MaterialPass::Opaque, materialResources, sceneUniformData, _mainDescriptors);

        _mainDeletionQueue.PushFunction([=]
        {
            DestroyBuffer(materialConstants);
        });
    }

    void VulkanRenderer::CreateImGuiDescriptorPool()
    {
        constexpr VkDescriptorPoolSize poolSizes[] =
        {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
        };

        VkDescriptorPoolCreateInfo poolInfo = { };
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
        poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
        poolInfo.pPoolSizes = poolSizes;

        CHECK_RESULT(vkCreateDescriptorPool(_device, &poolInfo, _allocator, &_imGuiDescriptorPool));

        _mainDeletionQueue.PushFunction([&]{ vkDestroyDescriptorPool(_device, _imGuiDescriptorPool, _allocator); });
    }

    void VulkanRenderer::Draw(const WindowProperties& props, const DrawContext& drawCommands)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if (_rebuildSwapchain)
        {
            ResizeSwapchain();
        }

        VulkanFrame& frame = GetFrame();
        CHECK_RESULT(vkWaitForFences(_device, 1, &frame.Fence, true, 1000000000));

        uint32_t swapchainImageIndex;
        VkResult result = vkAcquireNextImageKHR(_device, _swapchain, 1000000000, frame.SwapchainSemaphore, nullptr, &swapchainImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            _rebuildSwapchain = true;
            return;
        }

        frame.DeletionQueue.Flush();
        frame.Descriptors.Clear(_device);

        UpdateGlobalUniforms();

        CHECK_RESULT(vkResetFences(_device, 1, &frame.Fence));

        const VkCommandBuffer cmd = frame.CommandBuffer;

        CHECK_RESULT(vkResetCommandBuffer(cmd, 0));
        const VkCommandBufferBeginInfo cmdBeginInfo = VulkanInitializers::CommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        CHECK_RESULT(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

        VulkanUtils::TransitionImage(cmd, _colorImage.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        VulkanUtils::TransitionImage(cmd, _depthImage.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);

        DrawGeometry(cmd, drawCommands);

        // Prepare and copy color buffer into the active swapchain buffer.
        VulkanUtils::TransitionImage(cmd, _colorImage.Image, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        const VkImage& swapchainImage = _swapchainImages[swapchainImageIndex];
        VulkanUtils::TransitionImage(cmd, swapchainImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        VulkanUtils::CopyImageToImage(cmd, _colorImage.Image, swapchainImage,_drawExtent ,_swapchainExtent);

        // Prepare and draw ImGui into active swapchain buffer.
        VulkanUtils::TransitionImage(cmd, swapchainImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        VulkanUtils::TransitionImage(cmd, _colorImage.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        DrawImGui(cmd, _swapchainImageViews[swapchainImageIndex]);

        // Prepare and present active swapchain buffer.
        VulkanUtils::TransitionImage(cmd, swapchainImage, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        CHECK_RESULT(vkEndCommandBuffer(cmd));

        const VkCommandBufferSubmitInfo cmdInfo = VulkanInitializers::CommandBufferSubmitInfo(cmd);
        const VkSemaphoreSubmitInfo waitInfo = VulkanInitializers::SemaphoreSubmitInfo(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, frame.SwapchainSemaphore);
        const VkSemaphoreSubmitInfo signalInfo = VulkanInitializers::SemaphoreSubmitInfo(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, frame.RenderSemaphore);

        const VkSubmitInfo2 submitInfo = VulkanInitializers::SubmitInfo(&cmdInfo, &signalInfo, &waitInfo);

        CHECK_RESULT(vkQueueSubmit2(_queue, 1, &submitInfo, frame.Fence));

        {
            ANE_DEEP_PROFILE_SCOPE("vkQueuePresentKHR");

            VkPresentInfoKHR presentInfo = VulkanInitializers::PresentInfo();

            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &frame.RenderSemaphore;

            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &_swapchain;

            presentInfo.pImageIndices = &swapchainImageIndex;

            result = vkQueuePresentKHR(_queue, &presentInfo);
            if (result == VK_ERROR_OUT_OF_DATE_KHR)
            {
                _rebuildSwapchain = true;
                return;
            }
        }

        _frameIndex++;
    }

    void VulkanRenderer::DrawSky(VkCommandBuffer cmd)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        VulkanFrame& frame = GetFrame();

        VkDescriptorSet skyDescriptor = frame.Descriptors.Allocate(_device, _singleImageDataLayout, _allocator);
        {
            DescriptorWriter writer;
            writer.WriteImage(0, _cubeMap.ImageView, _samplerLinear, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
            writer.UpdateSet(_device, skyDescriptor);
        }

        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _skyPipeline);

        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 2, 1, &skyDescriptor, 0, nullptr);

        PushConstantBuffer pushConstants;
        Matrix4x4 modelMat = Matrix4x4::Identity();
        modelMat.SetPosition(CameraPosition);
        pushConstants.MVPMatrix = ViewProjection * modelMat; // VP
        pushConstants.ModelMatrix = Matrix4x4::Identity();
        pushConstants.VertexBuffer = _skyMesh->MeshBuffers.VertexBufferAddress;

        vkCmdPushConstants(cmd, _pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstantBuffer), &pushConstants);
        vkCmdBindIndexBuffer(cmd, _skyMesh->MeshBuffers.IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdDrawIndexed(cmd, _skyMesh->NumVertices, 1, 0, 0, 0);
    }

    void VulkanRenderer::DrawGeometry(VkCommandBuffer cmd, const DrawContext& drawCommands)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        VulkanFrame& frame = GetFrame();

        VkDescriptorSet appDescriptor = frame.Descriptors.Allocate(_device, _appDataLayout, _allocator);
        VkDescriptorSet sceneDescriptor = frame.Descriptors.Allocate(_device, _geometryDataLayout, _allocator);

        // Global descriptor set 1.
        {
            DescriptorWriter writer;
            writer.WriteBuffer(0, frame.AppDataBuffer.Buffer, sizeof(ApplicationData), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
            writer.UpdateSet(_device, appDescriptor);
        }
        // Global descriptor set 2.
        {
            DescriptorWriter writer;
            writer.WriteBuffer(0, frame.SceneDataBuffer.Buffer, sizeof(SceneData), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
            writer.UpdateSet(_device, sceneDescriptor);
        }

        _drawExtent.height = std::min(_swapchainExtent.height, _colorImage.ImageExtent.height);
        _drawExtent.width = std::min(_swapchainExtent.width, _colorImage.ImageExtent.width);

        VkRenderingAttachmentInfo colorAttachment = VulkanInitializers::AttachmentInfo(_colorImage.ImageView, &_mainWindowData.ClearValue, VK_IMAGE_LAYOUT_GENERAL);
        VkRenderingAttachmentInfo depthAttachment = VulkanInitializers::DepthAttachmentInfo(_depthImage.ImageView, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);
        VkRenderingInfo renderInfo = VulkanInitializers::RenderingInfo(_drawExtent, &colorAttachment, &depthAttachment);
        vkCmdBeginRendering(cmd, &renderInfo);

        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &appDescriptor, 0, nullptr);
        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 1, 1, &sceneDescriptor, 0, nullptr);

        VkViewport viewport;
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = (float)_drawExtent.width;
        viewport.height = (float)_drawExtent.height;
        viewport.minDepth = 0.f;
        viewport.maxDepth = 1.f;

        vkCmdSetViewport(cmd, 0, 1, &viewport);

        VkRect2D scissor;
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        scissor.extent.width = _drawExtent.width;
        scissor.extent.height = _drawExtent.height;

        vkCmdSetScissor(cmd, 0, 1, &scissor);

        DrawSky(cmd);

        ShaderPipeline* lastPipeline = nullptr;
        MaterialInstance* lastMaterial = nullptr;
        for (DrawCommand drawCommand : drawCommands.Commands)
        {
            // Use Filament in case material reference is null.
            MaterialInstance* material = drawCommand.Material != nullptr ? drawCommand.Material : &_filamentInstance;
            if (material != lastMaterial)
            {
                lastMaterial = material;
                if (material->ShaderPipeline != lastPipeline)
                {
                    lastPipeline = material->ShaderPipeline;

                    // Per-shader.
                    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, material->ShaderPipeline->Pipeline);
                    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, material->ShaderPipeline->Layout, 0, 1, &appDescriptor, 0, nullptr);
                    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, material->ShaderPipeline->Layout, 1, 1, &sceneDescriptor, 0, nullptr);
                }

                // Per-material.
                vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, material->ShaderPipeline->Layout, 2, 1, &material->MaterialSet, 0, nullptr);
            }

            PushConstantBuffer pushConstants;
            pushConstants.MVPMatrix = ViewProjection * drawCommand.ModelMatrix; // VP * M
            pushConstants.ModelMatrix = drawCommand.ModelMatrix;
            pushConstants.VertexBuffer = drawCommand.MeshBuffers.VertexBufferAddress;

            // Per-object.
            vkCmdPushConstants(cmd, _pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstantBuffer), &pushConstants);
            vkCmdBindIndexBuffer(cmd, drawCommand.MeshBuffers.IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(cmd, drawCommand.VertexCount, 1, 0, 0, 0);
        }

        #ifndef ANE_DIST
        DrawDebugGeometry(cmd, drawCommands);
        #endif

        vkCmdEndRendering(cmd);
    }

    // TODO: Move this into separate cpp
    void VulkanRenderer::DrawDebugGeometry(const VkCommandBuffer cmd, const DrawContext& drawCommands)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        VulkanFrame& frame = GetFrame();
        for (DebugDrawCommand drawCommand : drawCommands.DebugCommands)
        {
            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, drawCommand.LineList ? _debugLinePipeline : _debugTrianglePipeline);

            frame.DeletionQueue.PushFunction([=]
            {
                DestroyBuffer(drawCommand.MeshBuffers.IndexBuffer);
                DestroyBuffer(drawCommand.MeshBuffers.VertexBuffer);
            });

            PushConstantBuffer pushConstants;
            pushConstants.MVPMatrix = ViewProjection * drawCommand.ModelMatrix; // VP * M
            pushConstants.ModelMatrix = drawCommand.ModelMatrix;
            pushConstants.VertexBuffer = drawCommand.MeshBuffers.VertexBufferAddress;

            vkCmdPushConstants(cmd, _pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstantBuffer), &pushConstants);
            vkCmdBindIndexBuffer(cmd, drawCommand.MeshBuffers.IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(cmd, drawCommand.VertexCount, 1, 0, 0, 0);
        }
    }

    // TODO: Fully integrate ImGui into rendering loop.
    void VulkanRenderer::DrawImGui(const VkCommandBuffer cmd, const VkImageView targetImageView)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        const VkRenderingAttachmentInfo colorAttachment = VulkanInitializers::AttachmentInfo(targetImageView, nullptr, VK_IMAGE_LAYOUT_GENERAL);
        const VkRenderingInfo renderInfo = VulkanInitializers::RenderingInfo(_windowExtent, &colorAttachment, nullptr);

        vkCmdBeginRendering(cmd, &renderInfo);

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);

        vkCmdEndRendering(cmd);
    }

    void VulkanRenderer::UpdateGlobalUniforms()
    {
        VulkanFrame& frame = GetFrame();

        frame.AppData.Time = API::TIME;

        auto* appUniformData = (ApplicationData*)frame.AppDataBuffer.Allocation->GetMappedData();
        *appUniformData = frame.AppData;

        Vector3 ambientColor = Vector3(0.05f, 0.08f, 0.11f);
        Vector3 sunAngle = Vector3(50 * FMath::DEGREES_TO_RAD, -30 * FMath::DEGREES_TO_RAD, 0);
        Vector3 sunDirection = Quaternion::FromEulerAngles(sunAngle) * Vector3::ForwardVector();
        Vector3 sunColor = Vector3(1.0f, 0.9f, 0.67f);

        frame.SceneData.CameraPosition = Vector4(CameraPosition);
        frame.SceneData.AmbientColor = Vector4(ambientColor, 0);
        frame.SceneData.SunlightDirection = Vector4(SunDirection, 0);
        frame.SceneData.SunlightColor = Vector4(SunColor, 0);

        auto* sceneUniformData = (SceneData*)frame.SceneDataBuffer.Allocation->GetMappedData();
        *sceneUniformData = frame.SceneData;
    }

    void VulkanRenderer::CleanupVulkan()
    {
        // Calls vkDestroyPipeline, vkDestroyRenderPass, vkDestroySwapchainKHR and vkDestroySurfaceKHR.
        ImGui_ImplVulkanH_DestroyWindow(_instance, _device, &_mainWindowData, _allocator);

        for (VulkanFrame& frame : _frameData)
        {
            frame.DeletionQueue.Flush();
        }

        _mainDeletionQueue.Flush();

        DestroyMainBuffers();

        DestroySwapchain();

        vkDestroySurfaceKHR(_instance, _surface, _allocator);

        vmaDestroyAllocator(_vmaAllocator);

        vkDestroyDevice(_device, _allocator);

        if (enableValidationLayers) vkb::destroy_debug_utils_messenger(_instance, _debugMessenger, _allocator);
        vkDestroyInstance(_instance, _allocator);
    }

    void VulkanRenderer::CleanupImGui()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        AneImGui::Cleanup();
    }

    VulkanFrame& VulkanRenderer::GetFrame()
    {
        return _frameData[_frameIndex % MAX_FRAMES_IN_FLIGHT];
    }

    void VulkanRenderer::CreateVmaAllocator()
    {
        VmaAllocatorCreateInfo allocatorInfo = { };
        allocatorInfo.physicalDevice = _physicalDevice;
        allocatorInfo.device = _device;
        allocatorInfo.instance = _instance;
        allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

        CHECK_RESULT(vmaCreateAllocator(&allocatorInfo, &_vmaAllocator));
    }

    VmaBuffer VulkanRenderer::CreateBuffer(const size_t allocSize, const VkBufferUsageFlags usage, const VmaMemoryUsage memoryUsage)
    {
        VkBufferCreateInfo bufferInfo = { .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, .pNext = nullptr};

        bufferInfo.size = allocSize;
        bufferInfo.usage = usage;

        VmaAllocationCreateInfo vmaAllocInfo = { };
        vmaAllocInfo.usage = memoryUsage;
        vmaAllocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

        VmaBuffer newBuffer;
        CHECK_RESULT(vmaCreateBuffer(_vmaAllocator, &bufferInfo, &vmaAllocInfo, &newBuffer.Buffer, &newBuffer.Allocation, &newBuffer.Info));

        return newBuffer;
    }

    void VulkanRenderer::DestroyBuffer(const VmaBuffer& buffer)
    {
        vmaDestroyBuffer(_vmaAllocator, buffer.Buffer, buffer.Allocation);
    }

    VmaImage VulkanRenderer::CreateImage(const VkExtent3D size, const VkFormat format, const VkImageUsageFlags usage, const VkImageViewType viewType, const bool mipmaps)
    {
        VmaImage newImage;
        newImage.ImageFormat = format;
        newImage.ImageExtent = size;

        VkImageCreateInfo imgInfo = VulkanInitializers::ImageCreateInfo(format, usage, size);
        if (mipmaps)
        {
            imgInfo.mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(size.width, size.height)))) + 1;
        }

        const bool isCubeMap = viewType == VK_IMAGE_VIEW_TYPE_CUBE;
        if (isCubeMap)
        {
            imgInfo.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
            imgInfo.arrayLayers = 6;
        }

        // Always allocate images on dedicated GPU memory.
        VmaAllocationCreateInfo allocInfo = { };
        allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        allocInfo.requiredFlags = (VkMemoryPropertyFlags)VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        CHECK_RESULT(vmaCreateImage(_vmaAllocator, &imgInfo, &allocInfo, &newImage.Image, &newImage.Allocation, nullptr));

        // Depth formats need to use the correct aspect flag.
        VkImageAspectFlags aspectFlag = VK_IMAGE_ASPECT_COLOR_BIT;
        if (format == VK_FORMAT_D32_SFLOAT)
        {
            aspectFlag = VK_IMAGE_ASPECT_DEPTH_BIT;
        }

        // Build a image-view for the image.
        VkImageViewCreateInfo viewInfo = VulkanInitializers::ImageViewCreateInfo(format, newImage.Image, aspectFlag);
        viewInfo.viewType = viewType;
        viewInfo.subresourceRange.levelCount = imgInfo.mipLevels;

        if (isCubeMap)
        {
            viewInfo.subresourceRange.layerCount = 6;
        }

        CHECK_RESULT(vkCreateImageView(_device, &viewInfo, _allocator, &newImage.ImageView));

        return newImage;
    }

    VmaImage VulkanRenderer::CreateImage(const void* data, const VkExtent3D size, const VkFormat format, const VkImageUsageFlags usage, const VkImageViewType viewType, const bool mipmaps)
    {
        const size_t dataSize = (size_t)(size.depth * size.width * size.height * 4);
        const VmaBuffer uploadBuffer = CreateBuffer(dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

        memcpy(uploadBuffer.Info.pMappedData, data, dataSize);

        const VmaImage newImage = CreateImage(size, format, usage | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, viewType, mipmaps);

        ImmediateSubmit([&](const VkCommandBuffer cmd)
        {
            VulkanUtils::TransitionImage(cmd, newImage.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            VkBufferImageCopy copyRegion = { };
            copyRegion.bufferOffset = 0;
            copyRegion.bufferRowLength = 0;
            copyRegion.bufferImageHeight = 0;

            copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copyRegion.imageSubresource.mipLevel = 0;
            copyRegion.imageSubresource.baseArrayLayer = 0;
            copyRegion.imageSubresource.layerCount = 1;
            copyRegion.imageExtent = size;

            // Copy the buffer into the image.
            vkCmdCopyBufferToImage(cmd, uploadBuffer.Buffer, newImage.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

            //VulkanUtils::TransitionImage(cmd, newImage.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        });

        DestroyBuffer(uploadBuffer);

        uint32_t mipLevels = 1;
        if (mipmaps)
        {
            mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(size.width, size.height)))) + 1;
        }
        GenerateMipMaps(newImage, size, mipLevels);

        return newImage;
    }

    VmaImage VulkanRenderer::CreateCubeImage(const void* data, VkExtent3D size, VkFormat format, VkImageUsageFlags usage, VkImageViewType viewType, bool mipmaps)
    {
        const VkDeviceSize layerSize = (VkDeviceSize)size.depth * size.width * size.height * 4;
        const VkDeviceSize imageSize = layerSize * 6;

        const VmaBuffer uploadBuffer = CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

        memcpy(uploadBuffer.Info.pMappedData, data, imageSize);

        const VmaImage newImage = CreateImage(size, format, usage | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, viewType, mipmaps);

        ImmediateSubmit([&](const VkCommandBuffer cmd)
        {
            VulkanUtils::TransitionImage(cmd, newImage.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

            std::vector<VkBufferImageCopy> copyRegions;
            copyRegions.reserve(6);
            for (uint32_t layer = 0; layer < 6; layer++)
            {
                VkBufferImageCopy region = {};
                region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                region.imageSubresource.mipLevel = 0;
                region.imageSubresource.baseArrayLayer = layer;
                region.imageSubresource.layerCount = 1;
                region.imageExtent.width = size.width;
                region.imageExtent.height = size.height;
                region.imageExtent.depth = size.depth;
                region.bufferOffset = layerSize * layer;

                copyRegions.push_back(region);
            }

            // Copy the buffer into the image.
            vkCmdCopyBufferToImage(cmd, uploadBuffer.Buffer, newImage.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(copyRegions.size()), copyRegions.data());

            //VulkanUtils::TransitionImage(cmd, newImage.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        });

        DestroyBuffer(uploadBuffer);

        uint32_t mipLevels = 1;
        if (mipmaps)
        {
            mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(size.width, size.height)))) + 1;
        }
        GenerateMipMaps(newImage, size, mipLevels, true);

        return newImage;
    }

    void VulkanRenderer::DestroyImage(const VmaImage& image)
    {
        vkDestroyImageView(_device, image.ImageView, _allocator);
        vmaDestroyImage(_vmaAllocator, image.Image, image.Allocation);
    }

    void VulkanRenderer::GenerateMipMaps(const VmaImage& image, const VkExtent3D size, const uint32_t mipLevels, const bool cubemap)
    {
        ImmediateSubmit([&](const VkCommandBuffer cmd)
        {
            const uint32_t layerCount = cubemap ? 6 : 1;

            VkImageMemoryBarrier barrier = { };
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.image = image.Image;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.layerCount = 1;
            barrier.subresourceRange.levelCount = 1;

            for (uint32_t layerIndex = 0; layerIndex < layerCount; layerIndex++)
            {
                barrier.subresourceRange.baseArrayLayer = layerIndex;

                int32_t mipWidth = (int32_t)size.width;
                int32_t mipHeight = (int32_t)size.height;

                for (uint32_t i = 1; i < mipLevels; i++)
                {
                    barrier.subresourceRange.baseMipLevel = i - 1;

                    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

                    vkCmdPipelineBarrier(cmd,
                        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                        0, nullptr,
                        0, nullptr,
                        1, &barrier);

                    VkImageBlit blit{};
                    blit.srcOffsets[0] = { 0, 0, 0 };
                    blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
                    blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    blit.srcSubresource.mipLevel = i - 1;
                    blit.srcSubresource.baseArrayLayer = layerIndex;
                    blit.srcSubresource.layerCount = 1;
                    blit.dstOffsets[0] = { 0, 0, 0 };
                    blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
                    blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    blit.dstSubresource.mipLevel = i;
                    blit.dstSubresource.baseArrayLayer = layerIndex;
                    blit.dstSubresource.layerCount = 1;

                    vkCmdBlitImage(cmd,
                        image.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                        image.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        1, &blit,
                        VK_FILTER_LINEAR);

                    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                    vkCmdPipelineBarrier(cmd,
                        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                        0, nullptr,
                        0, nullptr,
                        1, &barrier);

                    if (mipWidth > 1) mipWidth /= 2;
                    if (mipHeight > 1) mipHeight /= 2;
                }

                barrier.subresourceRange.baseMipLevel = mipLevels - 1;
                barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                vkCmdPipelineBarrier(cmd,
                    VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                    0, nullptr,
                    0, nullptr,
                    1, &barrier);
            }
        });
    }

    void VulkanRenderer::ImmediateSubmit(std::function<void(VkCommandBuffer cmd)>&& function)
    {
        CHECK_RESULT(vkResetFences(_device, 1, &_immBuffer.Fence));
        CHECK_RESULT(vkResetCommandBuffer(_immBuffer.CommandBuffer, 0));

        const VkCommandBuffer cmd = _immBuffer.CommandBuffer;
        // The command buffer is used exactly once, so we let Vulkan know that.
        const VkCommandBufferBeginInfo cmdBeginInfo = VulkanInitializers::CommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        CHECK_RESULT(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

        function(cmd);

        CHECK_RESULT(vkEndCommandBuffer(cmd));

        const VkCommandBufferSubmitInfo cmdInfo = VulkanInitializers::CommandBufferSubmitInfo(cmd);
        const VkSubmitInfo2 submit = VulkanInitializers::SubmitInfo(&cmdInfo, nullptr, nullptr);

        CHECK_RESULT(vkQueueSubmit2(_queue, 1, &submit, _immBuffer.Fence));

        // Thread will be blocked until the graphic commands finish execution.
        CHECK_RESULT(vkWaitForFences(_device, 1, &_immBuffer.Fence, true, 9999999999));
    }

    VmaMeshBuffers VulkanRenderer::UploadMesh(const std::span<uint32_t> indices, const std::span<Vertex> vertices)
    {
        const size_t vertexBufferSize = vertices.size() * sizeof(Vertex);
        const size_t indexBufferSize = indices.size() * sizeof(uint32_t);

        VmaMeshBuffers newSurface;

        constexpr VkBufferUsageFlags vertexBufferFlags =
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
            VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
        newSurface.VertexBuffer = CreateBuffer(vertexBufferSize, vertexBufferFlags, VMA_MEMORY_USAGE_GPU_ONLY);

        // Find the address of the vertex buffer.
        const VkBufferDeviceAddressInfo deviceAddressInfo{ .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO, .buffer = newSurface.VertexBuffer.Buffer };
        newSurface.VertexBufferAddress = vkGetBufferDeviceAddress(_device, &deviceAddressInfo);

        constexpr VkBufferUsageFlags indexBufferFlags =
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
            VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        newSurface.IndexBuffer = CreateBuffer(indexBufferSize, indexBufferFlags, VMA_MEMORY_USAGE_GPU_ONLY);

        const VmaBuffer staging = CreateBuffer(vertexBufferSize + indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);

        // Copy buffers over to VMA allocation address.
        void* data = staging.Allocation->GetMappedData();
        memcpy(data, vertices.data(), vertexBufferSize);
        memcpy((char*)data + vertexBufferSize, indices.data(), indexBufferSize);

        // Copy buffers to the GPU.
        ImmediateSubmit([&](const VkCommandBuffer cmd)
        {
            VkBufferCopy vertexCopy;
            vertexCopy.srcOffset = 0;
            vertexCopy.dstOffset = 0;
            vertexCopy.size = vertexBufferSize;

            vkCmdCopyBuffer(cmd, staging.Buffer, newSurface.VertexBuffer.Buffer, 1, &vertexCopy);

            VkBufferCopy indexCopy;
            indexCopy.srcOffset = vertexBufferSize;
            indexCopy.dstOffset = 0;
            indexCopy.size = indexBufferSize;

            vkCmdCopyBuffer(cmd, staging.Buffer, newSurface.IndexBuffer.Buffer, 1, &indexCopy);
        });

        DestroyBuffer(staging);

        return newSurface;
    }

    VmaMeshBuffers VulkanRenderer::UploadRawMesh(const std::span<uint32_t> indices, const std::span<Vertex> vertices)
    {
        return UploadMesh(indices, vertices);
    }

    VkBool32 VulkanRenderer::DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            ANE_ELOG_ERROR("Validation layer: {0}", pCallbackData->pMessage);
        }
        else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            ANE_ELOG_WARN("Validation layer: {0}", pCallbackData->pMessage);
        }
        //else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        //{
        //    ANE_ELOG_INFO("Validation layer: {0}", pCallbackData->pMessage);
        //}
        //else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        //{
        //    ANE_ELOG_INFO("Validation layer: {0}", pCallbackData->pMessage);
        //}

        return VK_FALSE;
    }
}
