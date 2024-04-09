#include "anepch.h"
#include "VulkanRenderer.h"

#include <SDL.h>
#include <SDL_vulkan.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_vulkan.h>
#include <VkBootstrap.h>
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include "VulkanInitializers.h"
#include "VulkanUtils.h"
#include "ANE/Core/Window.h"
#include "ANE/Utilities/ImGuiUtilities.h"

namespace Engine
{
    VulkanRenderer::VulkanRenderer(SDL_Window* window)
    {
        _window = window;
    }

    void VulkanRenderer::Setup()
    {
        if (_initialized)
        {
            ANE_ENGINE_LOG_WARN("Unable to setup VulkanRenderer as it has already been initialized.");
            return;
        }

        SetupVulkan(_window);
        SetupImGui(_window);
        _initialized = true;
    }

    void VulkanRenderer::Render(const WindowProperties& props)
    {
        if (!_initialized)
        {
            ANE_ENGINE_LOG_WARN("Unable to end VulkanRenderer frame as it was never fully initialized.");
            return;
        }

        //ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
        if (!isMinimized)
        {
            _mainWindowData.ClearValue.color.float32[0] = ClearColor.x * ClearColor.w;
            _mainWindowData.ClearValue.color.float32[1] = ClearColor.y * ClearColor.w;
            _mainWindowData.ClearValue.color.float32[2] = ClearColor.z * ClearColor.w;
            _mainWindowData.ClearValue.color.float32[3] = ClearColor.w;
            Draw(props);
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
            ANE_ENGINE_LOG_WARN("Unable to clean up VulkanRenderer as it was never fully initialized.");
            return;
        }

        const VkResult err = vkDeviceWaitIdle(_device);
        CheckVkResult(err);

        CleanupImGui();
        CleanupVulkan();
        _initialized = false;
    }

    float VulkanRenderer::GetFramerate()
    {
        return _io->Framerate;
    }

    void VulkanRenderer::SetupVulkan(SDL_Window* window)
    {
        const std::vector<const char*> extensions = GetAvailableExtensions(window);
        vkb::Instance vkbInstance = CreateVkbInstance(extensions);
        _instance = vkbInstance.instance;
        _debugMessenger = vkbInstance.debug_messenger;

        if (SDL_Vulkan_CreateSurface(window, _instance, &_surface) == 0)
        {
            ANE_ENGINE_LOG_ERROR("Could not create Vulkan surface.\n");
        }

        const vkb::PhysicalDevice physicalDevice = SelectVkbPhysicalDevice(_surface, vkbInstance);
        _physicalDevice = physicalDevice.physical_device;

        const vkb::Device logicalDevice = CreateVkbLogicalDevice(physicalDevice);
        _device = logicalDevice.device;

        vkb::Result<VkQueue> queueResult = logicalDevice.get_queue(vkb::QueueType::graphics);
        if (!queueResult.has_value())
        {
            ANE_ENGINE_LOG_ERROR("Queue has no graphics support.\n");
        }
        _queue = queueResult.value();

        CreateVmaAllocator();

        SetupSwapchain();

        SetupCommandBuffers();

        SetupSyncStructures();

        CreatePipeline(logicalDevice);
        //const PipelineWrapper pipeline = CreatePipeline(logicalDevice);
        //_trianglePipeline = pipeline.Pipeline;
    }

    void VulkanRenderer::SetupImGui(SDL_Window* window)
    {
        IMGUI_CHECKVERSION();
        CreateImGuiDescriptorPool();
        ImGui::CreateContext();

        _io = &ImGui::GetIO(); (void)_io;
        _io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        _io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        _io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        //_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows. Doesn't seem to like custom swapchains.
        //_io->ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        //_io->ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        // Setup Dear ImGui style
        //ImGuiUtilities::StyleAnemoneDark();
        ImGui::StyleColorsDark();
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
        ImGui_ImplVulkanH_Window* wd = &_mainWindowData;

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
    }

    void VulkanRenderer::CheckVkResult(VkResult err)
    {
        if (err == VK_SUCCESS)
            return;
        ANE_ENGINE_LOG_ERROR("Vulkan Error: VkResult = {0}", (int)err);
        if (err < 0)
            abort();
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
        VkDebugUtilsMessageSeverityFlagBitsEXT debugMessageSeverityFlags = (VkDebugUtilsMessageSeverityFlagBitsEXT)(
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
        VkDebugUtilsMessageTypeFlagBitsEXT debugMessageTypeFlags = (VkDebugUtilsMessageTypeFlagBitsEXT)(
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

        // Use VkBootstrap to select a gpu.
        // We want a gpu that can write to the SDL surface and supports vulkan 1.3 with the correct features.
        vkb::PhysicalDeviceSelector selector{ instance };
        vkb::PhysicalDevice physicalDevice = selector
            .set_minimum_version(1, 3)

            .set_required_features_13(features)
            .set_required_features_12(features12)

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

            queueDescriptions.push_back(vkb::CustomQueueDescription (i, std::vector<float> (queueFamilies[i].queueCount, 1.0f)));
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
        int w, h;
        SDL_GetWindowSize(_window, &w, &h);
        _windowExtent.width = w;
        _windowExtent.height = h;

        vkb::Swapchain swapchain = CreateSwapchain(w, h);
        _swapchainExtent = swapchain.extent;
        _swapchain = swapchain.swapchain;
        _swapchainImages = swapchain.get_images().value();
        _swapchainImageViews = swapchain.get_image_views().value();

        const VkExtent3D imageExtent = { (uint32_t)w, (uint32_t)h, 1 };

        // Setup color buffer.
        _colorImage.ImageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
        _colorImage.ImageExtent = imageExtent;

        VkImageUsageFlags drawImageUsages{};
        drawImageUsages |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        drawImageUsages |= VK_IMAGE_USAGE_STORAGE_BIT;
        drawImageUsages |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const VkImageCreateInfo cImgInfo = VulkanInitializers::ImageCreateInfo(_colorImage.ImageFormat, drawImageUsages, imageExtent);

        VmaAllocationCreateInfo cImgAllocInfo = { };
        cImgAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        cImgAllocInfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        CheckVkResult(vmaCreateImage(_vmaAllocator, &cImgInfo, &cImgAllocInfo, &_colorImage.Image, &_colorImage.Allocation, nullptr));

        const VkImageViewCreateInfo cViewInfo = VulkanInitializers::ImageViewCreateInfo(_colorImage.ImageFormat, _colorImage.Image, VK_IMAGE_ASPECT_COLOR_BIT);

        CheckVkResult(vkCreateImageView(_device, &cViewInfo, _allocator, &_colorImage.ImageView));

        // Setup depth buffer.
        _depthImage.ImageFormat = VK_FORMAT_D32_SFLOAT;
        _depthImage.ImageExtent = imageExtent;

        VkImageUsageFlags depthImageUsages{};
        depthImageUsages |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        const VkImageCreateInfo dImgInfo = VulkanInitializers::ImageCreateInfo(_depthImage.ImageFormat, depthImageUsages, imageExtent);

        CheckVkResult(vmaCreateImage(_vmaAllocator, &dImgInfo, &cImgAllocInfo, &_depthImage.Image, &_depthImage.Allocation, nullptr));

        const VkImageViewCreateInfo dViewInfo = VulkanInitializers::ImageViewCreateInfo(_depthImage.ImageFormat, _depthImage.Image, VK_IMAGE_ASPECT_DEPTH_BIT);

        CheckVkResult(vkCreateImageView(_device, &dViewInfo, _allocator, &_depthImage.ImageView));

        // Cleanup.
        _mainDeletionQueue.PushFunction([=]
        {
            vkDestroyImageView(_device, _colorImage.ImageView, _allocator);
            vmaDestroyImage(_vmaAllocator, _colorImage.Image, _colorImage.Allocation);

            vkDestroyImageView(_device, _depthImage.ImageView, _allocator);
            vmaDestroyImage(_vmaAllocator, _depthImage.Image, _depthImage.Allocation);
        });
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

        int w, h;
        SDL_GetWindowSize(_window, &w, &h);
        _windowExtent.width = w;
        _windowExtent.height = h;

        vkb::Swapchain swapchain = CreateSwapchain(w, h);
        _swapchainExtent = swapchain.extent;
        _swapchain = swapchain.swapchain;
        _swapchainImages = swapchain.get_images().value();
        _swapchainImageViews = swapchain.get_image_views().value();

        _rebuildSwapchain = false;
    }

    void VulkanRenderer::SetupCommandBuffers()
    {
        const uint32_t familyIndex = _queueFamily.GraphicsFamily.value();
        const VkCommandPoolCreateInfo commandPoolInfo = VulkanInitializers::CommandPoolCreateInfo(familyIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

        // Frame buffers.
        for (VulkanFrame& frame : _frameData)
        {
            CheckVkResult(vkCreateCommandPool(_device, &commandPoolInfo, _allocator, &frame.CommandPool));

            VkCommandBufferAllocateInfo cmdAllocInfo = VulkanInitializers::CommandBufferAllocateInfo(frame.CommandPool, 1);

            CheckVkResult(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &frame.CommandBuffer));

            _mainDeletionQueue.PushFunction([=]
            {
                vkDestroyCommandPool(_device, frame.CommandPool, _allocator);
            });
        }

        // Immediate buffer.
        CheckVkResult(vkCreateCommandPool(_device, &commandPoolInfo, _allocator, &_immBuffer.CommandPool));

        const VkCommandBufferAllocateInfo cmdAllocInfo = VulkanInitializers::CommandBufferAllocateInfo(_immBuffer.CommandPool, 1);

        CheckVkResult(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &_immBuffer.CommandBuffer));

        _mainDeletionQueue.PushFunction([=]
        {
            vkDestroyCommandPool(_device, _immBuffer.CommandPool, _allocator);
        });
    }

    void VulkanRenderer::SetupSyncStructures()
    {
        const VkFenceCreateInfo fenceCreateInfo = VulkanInitializers::FenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
        CheckVkResult(vkCreateFence(_device, &fenceCreateInfo, _allocator, &_immBuffer.Fence));

        _mainDeletionQueue.PushFunction([=]
        {
            vkDestroyFence(_device, _immBuffer.Fence, _allocator);
        });

        for (VulkanFrame& frame : _frameData)
        {
            CheckVkResult(vkCreateFence(_device, &fenceCreateInfo, _allocator, &frame.Fence));

            VkSemaphoreCreateInfo semaphoreCreateInfo = VulkanInitializers::SemaphoreCreateInfo();

            CheckVkResult(vkCreateSemaphore(_device, &semaphoreCreateInfo, _allocator, &frame.SwapchainSemaphore));
            CheckVkResult(vkCreateSemaphore(_device, &semaphoreCreateInfo, _allocator, &frame.RenderSemaphore));

            _mainDeletionQueue.PushFunction([=]
            {
                vkDestroyFence(_device, frame.Fence, _allocator);
                vkDestroySemaphore(_device, frame.SwapchainSemaphore, _allocator);
                vkDestroySemaphore(_device, frame.RenderSemaphore, _allocator);
            });
        }
    }

    PipelineWrapper VulkanRenderer::CreatePipeline(const vkb::Device& logicalDevice)
    {
        VkShaderModule meshVertShader;
        if (!VulkanUtils::LoadShaderModule("../shaders/triangle_mesh.vert.spv", _device, _allocator, &meshVertShader))
        {
            ANE_ENGINE_LOG_ERROR("Error when building the triangle vertex shader module");
        }
        else
        {
            ANE_ENGINE_LOG_INFO("Loaded shader module: vertex_color.vert.spv");
        }

        VkShaderModule meshFragShader;
        if (!VulkanUtils::LoadShaderModule("../shaders/triangle_mesh.frag.spv", _device, _allocator, &meshFragShader))
        {
            ANE_ENGINE_LOG_ERROR("Error when building the triangle fragment shader module.");
        }
        else
        {
            ANE_ENGINE_LOG_INFO("Loaded shader module: vertex_color.frag.spv");
        }

        VkPushConstantRange bufferRange;
        bufferRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        bufferRange.offset = 0;
        bufferRange.size = sizeof(PushConstantBuffer);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = VulkanInitializers::PipelineLayoutCreateInfo();
        pipelineLayoutInfo.pPushConstantRanges = &bufferRange;
        pipelineLayoutInfo.pushConstantRangeCount = 1;

        CheckVkResult(vkCreatePipelineLayout(_device, &pipelineLayoutInfo, _allocator, &_pipelineLayout));

        VulkanPipelineBuilder builder{ logicalDevice, _pipelineLayout };
        vkb::Result<PipelineWrapper> pipeline = builder
            .SetShaders(meshVertShader, meshFragShader)
            .SetBlendMode(None)
            .SetDepthTestOperator(VK_COMPARE_OP_ALWAYS)

            .SetColorFormat(_colorImage.ImageFormat)
            .SetDepthFormat(_depthImage.ImageFormat)

            .SetAllocationCallbacks(_allocator)
            .Build();
        _meshPipeline = pipeline->Pipeline;

        // Cleanup.
        vkDestroyShaderModule(_device, meshFragShader, _allocator);
        vkDestroyShaderModule(_device, meshVertShader, _allocator);

        _mainDeletionQueue.PushFunction([&]
        {
            vkDestroyPipelineLayout(_device, _pipelineLayout, _allocator);
            vkDestroyPipeline(_device, _meshPipeline, _allocator);
        });

        return pipeline.value();
    }

    void VulkanRenderer::CreateImGuiDescriptorPool()
    {
        VkDescriptorPoolSize poolSizes[] =
        {
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
        };

        VkDescriptorPoolCreateInfo poolInfo = { };
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1;
        poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
        poolInfo.pPoolSizes = poolSizes;

        VkResult err = vkCreateDescriptorPool(_device, &poolInfo, _allocator, &_imGuiDescriptorPool);
        CheckVkResult(err);

        _mainDeletionQueue.PushFunction([&]{ vkDestroyDescriptorPool(_device, _imGuiDescriptorPool, _allocator); });
    }

    void VulkanRenderer::Draw(const WindowProperties& props)
    {
        if (_rebuildSwapchain)
        {
            if (props.Width > 0 && props.Height > 0)
            {
                ResizeSwapchain();
            }
        }

        ImDrawData* drawData = ImGui::GetDrawData();
        const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
        if (isMinimized)
        {
            return;
        }

        const VulkanFrame frame = GetFrame();
        CheckVkResult(vkWaitForFences(_device, 1, &frame.Fence, true, 1000000000));

        uint32_t swapchainImageIndex;
        VkResult result = vkAcquireNextImageKHR(_device, _swapchain, 1000000000, frame.SwapchainSemaphore, nullptr, &swapchainImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            _rebuildSwapchain = true;
            return;
        }

        CheckVkResult(vkResetFences(_device, 1, &frame.Fence));

        const VkCommandBuffer cmd = frame.CommandBuffer;

        CheckVkResult(vkResetCommandBuffer(cmd, 0));
        const VkCommandBufferBeginInfo cmdBeginInfo = VulkanInitializers::CommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        CheckVkResult(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

        VulkanUtils::TransitionImage(cmd, _colorImage.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
        VulkanUtils::TransitionImage(cmd, _depthImage.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);

        // TODO: Draw geometry here.

        VulkanUtils::TransitionImage(cmd, _colorImage.Image, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        const VkImage& swapchainImage = _swapchainImages[swapchainImageIndex];
        VulkanUtils::TransitionImage(cmd, swapchainImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        VulkanUtils::TransitionImage(cmd, swapchainImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        DrawImGui(cmd, _swapchainImageViews[swapchainImageIndex]);

        VulkanUtils::TransitionImage(cmd, swapchainImage, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        CheckVkResult(vkEndCommandBuffer(cmd));

        const VkCommandBufferSubmitInfo cmdInfo = VulkanInitializers::CommandBufferSubmitInfo(cmd);
        const VkSemaphoreSubmitInfo waitInfo = VulkanInitializers::SemaphoreSubmitInfo(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, frame.SwapchainSemaphore);
        const VkSemaphoreSubmitInfo signalInfo = VulkanInitializers::SemaphoreSubmitInfo(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, frame.RenderSemaphore);

        const VkSubmitInfo2 submitInfo = VulkanInitializers::SubmitInfo(&cmdInfo, &signalInfo, &waitInfo);

        CheckVkResult(vkQueueSubmit2(_queue, 1, &submitInfo, frame.Fence));

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

        _frameIndex++;
    }

    void VulkanRenderer::DrawGeometry(VkCommandBuffer cmd)
    {
        _drawExtent.height = std::min(_swapchainExtent.height, _colorImage.ImageExtent.height);
        _drawExtent.width = std::min(_swapchainExtent.width, _colorImage.ImageExtent.width);

        VkRenderingAttachmentInfo colorAttachment = VulkanInitializers::AttachmentInfo(_colorImage.ImageView, &_mainWindowData.ClearValue, VK_IMAGE_LAYOUT_GENERAL);
        VkRenderingAttachmentInfo depthAttachment = VulkanInitializers::DepthAttachmentInfo(_depthImage.ImageView, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);
        VkRenderingInfo renderInfo = VulkanInitializers::RenderingInfo(_drawExtent, &colorAttachment, &depthAttachment);
        vkCmdBeginRendering(cmd, &renderInfo);

        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _meshPipeline);

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

        vkCmdDraw(cmd, 3, 1, 0, 0);

        vkCmdEndRendering(cmd);
    }

    // TODO: Fully integrate ImGui into rendering loop.
    void VulkanRenderer::DrawImGui(VkCommandBuffer cmd, VkImageView targetImageView)
    {
        VkRenderingAttachmentInfo colorAttachment = VulkanInitializers::AttachmentInfo(targetImageView, &_mainWindowData.ClearValue, VK_IMAGE_LAYOUT_GENERAL);
        VkRenderingInfo renderInfo = VulkanInitializers::RenderingInfo(_windowExtent, &colorAttachment, nullptr);

        vkCmdBeginRendering(cmd, &renderInfo);

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);

        vkCmdEndRendering(cmd);
    }

    void VulkanRenderer::CleanupVulkan()
    {
        // Calls vkDestroyPipeline, vkDestroyRenderPass, vkDestroySwapchainKHR and vkDestroySurfaceKHR.
        ImGui_ImplVulkanH_DestroyWindow(_instance, _device, &_mainWindowData, _allocator);

        _mainDeletionQueue.Flush();

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
        ImGui::DestroyContext();
    }

    VulkanFrame VulkanRenderer::GetFrame()
    {
        return _frameData[_frameIndex % 3];
    }

    void VulkanRenderer::CreateVmaAllocator()
    {
        VmaAllocatorCreateInfo allocatorInfo = { };
        allocatorInfo.physicalDevice = _physicalDevice;
        allocatorInfo.device = _device;
        allocatorInfo.instance = _instance;
        allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

        CheckVkResult(vmaCreateAllocator(&allocatorInfo, &_vmaAllocator));
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
        CheckVkResult(vmaCreateBuffer(_vmaAllocator, &bufferInfo, &vmaAllocInfo, &newBuffer.Buffer, &newBuffer.Allocation, &newBuffer.Info));

        return newBuffer;
    }

    void VulkanRenderer::DestroyBuffer(const VmaBuffer& buffer)
    {
        vmaDestroyBuffer(_vmaAllocator, buffer.Buffer, buffer.Allocation);
    }

    VmaImage VulkanRenderer::CreateImage(const VkExtent3D size, const VkFormat format, const VkImageUsageFlags usage, const bool mipmaps)
    {
        VmaImage newImage;
        newImage.ImageFormat = format;
        newImage.ImageExtent = size;

        VkImageCreateInfo imgInfo = VulkanInitializers::ImageCreateInfo(format, usage, size);
        if (mipmaps)
        {
            imgInfo.mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(size.width, size.height)))) + 1;
        }

        // Always allocate images on dedicated GPU memory.
        VmaAllocationCreateInfo allocInfo = { };
        allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        allocInfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        CheckVkResult(vmaCreateImage(_vmaAllocator, &imgInfo, &allocInfo, &newImage.Image, &newImage.Allocation, nullptr));

        // Depth formats need to use the correct aspect flag.
        VkImageAspectFlags aspectFlag = VK_IMAGE_ASPECT_COLOR_BIT;
        if (format == VK_FORMAT_D32_SFLOAT)
        {
            aspectFlag = VK_IMAGE_ASPECT_DEPTH_BIT;
        }

        // Build a image-view for the image.
        VkImageViewCreateInfo viewInfo = VulkanInitializers::ImageViewCreateInfo(format, newImage.Image, aspectFlag);
        viewInfo.subresourceRange.levelCount = imgInfo.mipLevels;

        CheckVkResult(vkCreateImageView(_device, &viewInfo, _allocator, &newImage.ImageView));

        return newImage;
    }

    VmaImage VulkanRenderer::CreateImage(const void* data, const VkExtent3D size, const VkFormat format, const VkImageUsageFlags usage, const bool mipmaps)
    {
        const size_t dataSize = (size_t)(size.depth * size.width * size.height * 4);
        const VmaBuffer uploadBuffer = CreateBuffer(dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

        memcpy(uploadBuffer.Info.pMappedData, data, dataSize);

        const VmaImage newImage = CreateImage(size, format, usage | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, mipmaps);

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

            VulkanUtils::TransitionImage(cmd, newImage.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        });

        DestroyBuffer(uploadBuffer);

        return newImage;
    }

    void VulkanRenderer::DestroyImage(const VmaImage& image)
    {
        vkDestroyImageView(_device, image.ImageView, _allocator);
        vmaDestroyImage(_vmaAllocator, image.Image, image.Allocation);
    }

    void VulkanRenderer::ImmediateSubmit(std::function<void(VkCommandBuffer cmd)>&& function)
    {
        CheckVkResult(vkResetFences(_device, 1, &_immBuffer.Fence));
        CheckVkResult(vkResetCommandBuffer(_immBuffer.CommandBuffer, 0));

        const VkCommandBuffer cmd = _immBuffer.CommandBuffer;
        // The command buffer is used exactly once, so we let Vulkan know that.
        const VkCommandBufferBeginInfo cmdBeginInfo = VulkanInitializers::CommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        CheckVkResult(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

        function(cmd);

        CheckVkResult(vkEndCommandBuffer(cmd));

        const VkCommandBufferSubmitInfo cmdInfo = VulkanInitializers::CommandBufferSubmitInfo(cmd);
        const VkSubmitInfo2 submit = VulkanInitializers::SubmitInfo(&cmdInfo, nullptr, nullptr);

        CheckVkResult(vkQueueSubmit2(_queue, 1, &submit, _immBuffer.Fence));

        // Thread will be blocked until the graphic commands finish execution.
        CheckVkResult(vkWaitForFences(_device, 1, &_immBuffer.Fence, true, 9999999999));
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

    VkBool32 VulkanRenderer::DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            ANE_ENGINE_LOG_ERROR("Validation layer: {0}", pCallbackData->pMessage);
        }
        else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            ANE_ENGINE_LOG_WARN("Validation layer: {0}", pCallbackData->pMessage);
        }
        //else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        //{
        //    ANE_ENGINE_LOG_INFO("Validation layer: {0}", pCallbackData->pMessage);
        //}
        //else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        //{
        //    ANE_ENGINE_LOG_INFO("Validation layer: {0}", pCallbackData->pMessage);
        //}

        return VK_FALSE;
    }
}
