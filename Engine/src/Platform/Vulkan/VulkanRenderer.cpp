#include "nppch.h"
#include "VulkanRenderer.h"

#include <SDL.h>
#include <SDL_vulkan.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_vulkan.h>
#include <VkBootstrap.h>

#include "../../NP/Core/Window.h"

namespace Engine
{
    VulkanRenderer::VulkanRenderer(SDL_Window* window) : _window(window)
    {

    }

    void VulkanRenderer::Setup()
    {
        if (_initialized)
        {
            NP_ENGINE_LOG_WARN("Unable to setup VulkanRenderer as it has already been initialized.");
            return;
        }

        SetupVulkan(_window);
        SetupImGui(_window);
        _initialized = true;
    }

    void VulkanRenderer::NewFrame(const WindowProperties& props)
    {
        if (!_initialized)
        {
            NP_ENGINE_LOG_WARN("Unable to begin VulkanRenderer frame as it was never fully initialized.");
            return;
        }

        if (_swapChainRebuild)
        {
            if (props.Width > 0 && props.Height > 0)
            {
                ImGui_ImplVulkan_SetMinImageCount(_minImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(_instance, _physicalDevice, _device, &_mainWindowData, _queueFamily.GraphicsFamily.value(), _allocator, props.Width, props.Height, _minImageCount);
                _mainWindowData.FrameIndex = 0;
                _swapChainRebuild = false;
            }
        }

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void VulkanRenderer::EndFrame()
    {
        if (!_initialized)
        {
            NP_ENGINE_LOG_WARN("Unable to end VulkanRenderer frame as it was never fully initialized.");
            return;
        }

        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
        if (!isMinimized)
        {
            _mainWindowData.ClearValue.color.float32[0] = ClearColor.x * ClearColor.w;
            _mainWindowData.ClearValue.color.float32[1] = ClearColor.y * ClearColor.w;
            _mainWindowData.ClearValue.color.float32[2] = ClearColor.z * ClearColor.w;
            _mainWindowData.ClearValue.color.float32[3] = ClearColor.w;
            RenderFrame(&_mainWindowData, drawData);
            RevealFrame(&_mainWindowData);
        }
    }

    void VulkanRenderer::Cleanup()
    {
        if (!_initialized)
        {
            NP_ENGINE_LOG_WARN("Unable to clean up VulkanRenderer as it was never fully initialized.");
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
            NP_ENGINE_LOG_ERROR("Could not create Vulkan surface.\n");
        }

        const vkb::PhysicalDevice physicalDevice = SelectVkbPhysicalDevice(_surface, vkbInstance);
        _physicalDevice = physicalDevice.physical_device;

        const vkb::Device logicalDevice = CreateVkbLogicalDevice(physicalDevice);
        _device = logicalDevice.device;

        vkb::Result<VkQueue> queueResult = logicalDevice.get_queue(vkb::QueueType::graphics);
        if (!queueResult.has_value())
        {
            NP_ENGINE_LOG_ERROR("Queue has no graphics support.\n");
        }
        _queue = queueResult.value();

        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        ImGui_ImplVulkanH_Window* wd = &_mainWindowData;
        SetupVulkanWindow(wd, _surface, w, h);

        CreateDescriptorPool();
    }

    void VulkanRenderer::SetupImGui(SDL_Window* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        _io = &ImGui::GetIO(); (void)_io;
        _io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        _io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplVulkanH_Window* wd = &_mainWindowData;
        ImGui_ImplSDL2_InitForVulkan(window);
        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = _instance;
        initInfo.PhysicalDevice = _physicalDevice;
        initInfo.Device = _device;
        initInfo.QueueFamily = _queueFamily.GraphicsFamily.value();
        initInfo.Queue = _queue;
        initInfo.PipelineCache = _pipelineCache;
        initInfo.DescriptorPool = _descriptorPool;
        initInfo.RenderPass = wd->RenderPass;
        initInfo.Subpass = 0;
        initInfo.MinImageCount = _minImageCount;
        initInfo.ImageCount = wd->ImageCount;
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        initInfo.Allocator = _allocator;
        initInfo.CheckVkResultFn = CheckVkResult;
        ImGui_ImplVulkan_Init(&initInfo);
    }

    void VulkanRenderer::CheckVkResult(VkResult err)
    {
        if (err == VK_SUCCESS)
            return;
        NP_ENGINE_LOG_ERROR("Vulkan Error: VkResult = {0}", (int)err);
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

    void VulkanRenderer::SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height)
    {
        wd->Surface = surface;

        // Check for WSI support
        VkBool32 res;
        vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, _queueFamily.GraphicsFamily.value(), wd->Surface, &res);
        if (res != VK_TRUE)
        {
            NP_ENGINE_LOG_ERROR("Error no WSI support on physical device 0\n");
        }

        // Select Surface Format
        constexpr VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
        constexpr VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(_physicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

        // Select Present Mode
        #ifdef APP_UNLIMITED_FRAME_RATE
            VkPresentModeKHR presentModes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
        #else
            VkPresentModeKHR presentModes[] = { VK_PRESENT_MODE_FIFO_KHR };
        #endif
        wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(_physicalDevice, wd->Surface, &presentModes[0], IM_ARRAYSIZE(presentModes));
        //NP_ENGINE_LOG_INFO("Vulkan Info: Selected PresentMode = {0}", wd->PresentMode);

        // Create SwapChain, RenderPass, Framebuffer, etc.
        IM_ASSERT(_minImageCount >= 2);
        ImGui_ImplVulkanH_CreateOrResizeWindow(_instance, _physicalDevice, _device, wd, _queueFamily.GraphicsFamily.value(), _allocator, width, height, _minImageCount);
    }

    void VulkanRenderer::CreateDescriptorPool()
    {
        VkDescriptorPoolSize poolSizes[] =
        {
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
        };

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1;
        poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
        poolInfo.pPoolSizes = poolSizes;

        VkResult err = vkCreateDescriptorPool(_device, &poolInfo, _allocator, &_descriptorPool);
        CheckVkResult(err);
    }

    // TODO: Figure out what most of this code does.
    void VulkanRenderer::RenderFrame(ImGui_ImplVulkanH_Window* wd, ImDrawData* drawData)
    {
        VkSemaphore imageAcquiredSemaphore  = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
        VkSemaphore renderCompleteSemaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
        VkResult err = vkAcquireNextImageKHR(_device, wd->Swapchain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &wd->FrameIndex);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            _swapChainRebuild = true;
            return;
        }
        CheckVkResult(err);

        ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
        {
            err = vkWaitForFences(_device, 1, &fd->Fence, VK_TRUE, UINT64_MAX); // Wait indefinitely instead of periodically checking
            CheckVkResult(err);

            err = vkResetFences(_device, 1, &fd->Fence);
            CheckVkResult(err);
        }
        {
            err = vkResetCommandPool(_device, fd->CommandPool, 0);
            CheckVkResult(err);

            VkCommandBufferBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
            CheckVkResult(err);
        }
        {
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = wd->RenderPass;
            info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = wd->Width;
            info.renderArea.extent.height = wd->Height;
            info.clearValueCount = 1;
            info.pClearValues = &wd->ClearValue;

            vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
        }

        // Record dear imgui primitives into command buffer.
        ImGui_ImplVulkan_RenderDrawData(drawData, fd->CommandBuffer);

        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
        {
            VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

            VkSubmitInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores = &imageAcquiredSemaphore;
            info.pWaitDstStageMask = &waitStage;
            info.commandBufferCount = 1;
            info.pCommandBuffers = &fd->CommandBuffer;
            info.signalSemaphoreCount = 1;
            info.pSignalSemaphores = &renderCompleteSemaphore;

            err = vkEndCommandBuffer(fd->CommandBuffer);
            CheckVkResult(err);
            err = vkQueueSubmit(_queue, 1, &info, fd->Fence);
            CheckVkResult(err);
        }
    }

    // TODO: Figure out what most of this code does.
    void VulkanRenderer::RevealFrame(ImGui_ImplVulkanH_Window* wd)
    {
        if (_swapChainRebuild) return;

        VkSemaphore renderCompleteSemaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;

        VkPresentInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &renderCompleteSemaphore;
        info.swapchainCount = 1;
        info.pSwapchains = &wd->Swapchain;
        info.pImageIndices = &wd->FrameIndex;

        VkResult err = vkQueuePresentKHR(_queue, &info);
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            _swapChainRebuild = true;
            return;
        }
        CheckVkResult(err);

        wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->SemaphoreCount; // Now we can use the next set of semaphores
    }

    void VulkanRenderer::CleanupVulkan()
    {
        // Calls vkDestroyPipeline, vkDestroyRenderPass, vkDestroySwapchainKHR and vkDestroySurfaceKHR.
        ImGui_ImplVulkanH_DestroyWindow(_instance, _device, &_mainWindowData, _allocator);

        vkDestroyDescriptorPool(_device, _descriptorPool, _allocator);

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

    VkBool32 VulkanRenderer::DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            NP_ENGINE_LOG_ERROR("Validation layer: {0}", pCallbackData->pMessage);
        }
        else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            NP_ENGINE_LOG_WARN("Validation layer: {0}", pCallbackData->pMessage);
        }
        //else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        //{
        //    NP_ENGINE_LOG_INFO("Validation layer: {0}", pCallbackData->pMessage);
        //}
        //else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        //{
        //    NP_ENGINE_LOG_INFO("Validation layer: {0}", pCallbackData->pMessage);
        //}

        return VK_FALSE;
    }
}
