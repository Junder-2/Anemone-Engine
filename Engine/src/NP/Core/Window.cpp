#include "nppch.h"
#include "Window.h"

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_vulkan.h>
#include <VkBootstrap.h>

#include "Application.h"

namespace Engine
{
    Window::Window(const WindowProperties& props)
    {
        _windowData = props;
        Init(props);
    }

    Window::~Window()
    {
        Shutdown();
    }

    void Window::Init(const WindowProperties& props)
    {
        NP_ENGINE_LOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        // SDL
        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER))
        {
            NP_ENGINE_LOG_ERROR("Could not initialize SDL.");
        }

        SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        _windowContext = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowData.Width, _windowData.Height, windowFlags);
        if (_windowContext == nullptr)
        {
            NP_ENGINE_LOG_ERROR("Could not create SDL window.");
        }

        // Vulkan
        SetupVulkan(_windowContext);

        // Setup Dear ImGui context
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
        ImGui_ImplSDL2_InitForVulkan(_windowContext);
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

    void Window::OnUpdate(float deltaTime)
    {
        ProcessEvents(deltaTime);

        // Resize swap chain?
        if (_swapChainRebuild)
        {
            if (_windowData.Width > 0 && _windowData.Height > 0)
            {
                ImGui_ImplVulkan_SetMinImageCount(_minImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(_instance, _physicalDevice, _device, &_mainWindowData, _queueFamily.GraphicsFamily.value(), _allocator, _windowData.Width, _windowData.Height, _minImageCount);
                _mainWindowData.FrameIndex = 0;
                _swapChainRebuild = false;
            }
        }

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (_showDemoWindow)
            ImGui::ShowDemoWindow(&_showDemoWindow);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &_showDemoWindow); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &_showAnotherWindow);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&_clearColor); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / _io->Framerate, _io->Framerate);
        //ImGui::Text("Engine %.3f ms/frame", deltaTime * 1000.f);
        ImGui::End();

        // 3. Show another simple window.
        if (_showAnotherWindow)
        {
            ImGui::Begin("Another Window", &_showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                _showAnotherWindow = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
        if (!isMinimized)
        {
            _mainWindowData.ClearValue.color.float32[0] = _clearColor.x * _clearColor.w;
            _mainWindowData.ClearValue.color.float32[1] = _clearColor.y * _clearColor.w;
            _mainWindowData.ClearValue.color.float32[2] = _clearColor.z * _clearColor.w;
            _mainWindowData.ClearValue.color.float32[3] = _clearColor.w;
            RenderFrame(&_mainWindowData, drawData);
            RevealFrame(&_mainWindowData);
        }
    }

    void Window::ProcessEvents(float deltaTime)
    {
        InputManager* inputManager = &Application::Get().GetInputManager();
        inputManager->OnUpdate();

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            switch (event.type)
            {
                case SDL_QUIT:
                    if(WindowCloseDelegate) WindowCloseDelegate();
                break;
                case SDL_WINDOWEVENT:
                    ProcessWindowEvent(event.window, deltaTime);
                continue;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    if(event.key.repeat != 0) continue;
                    inputManager->ProcessKey(event.key.keysym.sym, event.type == SDL_KEYDOWN);
                continue;
                case SDL_MOUSEBUTTONDOWN:
                    case SDL_MOUSEBUTTONUP:
                    inputManager->ProcessMouseButton(MOUSE_BUTTON_TO_SDL_MOUSE_BUTTON(event.button.button), event.type == SDL_MOUSEBUTTONDOWN, event.button.clicks == 2);
                continue;
                case SDL_MOUSEMOTION:
                    inputManager->ProcessMouseMovement((float)event.motion.x/(float)_windowData.Width, (float)event.motion.y/(float)_windowData.Height, deltaTime);
                continue;
            }
        }
    }

    void Window::ProcessWindowEvent(const SDL_WindowEvent& windowEvent, float deltaTime)
    {
        switch (windowEvent.event)
        {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                if(_windowData.Width != windowEvent.data1 || _windowData.Height != windowEvent.data2) break;

                _windowData.Width = windowEvent.data1;
                _windowData.Height = windowEvent.data2;

                if(WindowResizeDelegate) WindowResizeDelegate(_windowData.Width, _windowData.Height);
            break;
        }
    }

    void Window::Shutdown()
    {
        // Cleanup
        const VkResult err = vkDeviceWaitIdle(_device);
        CheckVkResult(err);
        CleanupImGui();

        CleanupVulkan();

        SDL_DestroyWindow(_windowContext);
        SDL_Quit();
    }

    void Window::SetVSync(const bool enabled)
    {
        //todo
        _windowData.VSync = enabled;
    }

    // ImGui
    void Window::CleanupImGui()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    // Vulkan
    void Window::CheckVkResult(VkResult err)
    {
        if (err == VK_SUCCESS)
            return;
        NP_ENGINE_LOG_ERROR("Vulkan Error: VkResult = {0}", (int)err);
        if (err < 0)
            abort();
    }

    void Window::SetupVulkan(SDL_Window* window)
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

        // Select Physical Device (GPU)
        //_physicalDevice = SelectPhysicalDevice();

        //_queueFamily = FindQueueFamilies(_physicalDevice);

        // Create Logical Device (with 1 queue)
        //CreateLogicalDevice();

        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        ImGui_ImplVulkanH_Window* wd = &_mainWindowData;
        SetupVulkanWindow(wd, _surface, w, h);

        // Create Descriptor Pool
        CreateDescriptorPool();
    }

    std::vector<const char*> Window::GetAvailableExtensions(SDL_Window* window)
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

    bool Window::CheckValidationLayers()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        ImVector<VkLayerProperties> layers;
        layers.resize(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, layers.Data);

        for (const char* layerName : validationLayers)
        {
            bool layerFound = false;

            for (const VkLayerProperties& layerProps : layers)
            {
                if (strcmp(layerProps.layerName, layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                 return false;
            }
        }

        // Every layer is supported.
        return true;
    }

    vkb::Instance Window::CreateVkbInstance(const std::vector<const char*>& extensions)
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

    vkb::PhysicalDevice Window::SelectVkbPhysicalDevice(VkSurfaceKHR surface, vkb::Instance instance)
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

    vkb::Device Window::CreateVkbLogicalDevice(const vkb::PhysicalDevice& physicalDevice)
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

    VkPhysicalDevice Window::SelectPhysicalDevice()
    {
        uint32_t gpuCount;
        VkResult err = vkEnumeratePhysicalDevices(_instance, &gpuCount, nullptr);
        CheckVkResult(err);
        IM_ASSERT(gpuCount > 0);

        ImVector<VkPhysicalDevice> gpus;
        gpus.resize(gpuCount);
        err = vkEnumeratePhysicalDevices(_instance, &gpuCount, gpus.Data);
        CheckVkResult(err);

        // If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
        // most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
        // dedicated GPUs) is out of scope of this sample. - example_sdl2_vulkan
        for (const VkPhysicalDevice& device : gpus)
        {
            if (IsDeviceCompatible(device))
                return device;
        }

        // Use first GPU (Integrated) is a Discrete one is not available.
        if (gpuCount > 0)
            return gpus[0];
        return VK_NULL_HANDLE;
    }

    // TODO: Add more checks for features we depend on.
    bool Window::IsDeviceCompatible(const VkPhysicalDevice& device)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);

        const bool isDiscrete = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
        const bool hasFamily = FindQueueFamilies(device).HasFamily();
        return isDiscrete && hasFamily;
    }

    // TODO: Compute score based on available memory, supported types, max texture sizes etc.
    int Window::GetDeviceScore(const VkPhysicalDevice& device)
    {
        return 0;
    }

    QueueFamilyIndices Window::FindQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;

        uint32_t queueCount;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);
        ImVector<VkQueueFamilyProperties> queueFamilies;
        queueFamilies.resize(queueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queueFamilies.Data);
        for (uint32_t i = 0; i < queueCount; i++)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.GraphicsFamily = i;
                break;
            }
        }

        return indices;
    }

    void Window::CreateLogicalDevice()
    {
        constexpr float priority = 1.0f;
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = _queueFamily.GraphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &priority;

        VkPhysicalDeviceFeatures deviceFeatures = {};

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;

        createInfo.pEnabledFeatures = &deviceFeatures;

        // Extensions
        ImVector<const char*> deviceExtensions;
        deviceExtensions.push_back("VK_KHR_swapchain");

        createInfo.enabledExtensionCount = (uint32_t)deviceExtensions.Size;
        createInfo.ppEnabledExtensionNames = deviceExtensions.Data;

        // Layers
        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        VkResult err = vkCreateDevice(_physicalDevice, &createInfo, _allocator, &_device);
        CheckVkResult(err);
        vkGetDeviceQueue(_device, _queueFamily.GraphicsFamily.value(), 0, &_queue);
    }

    void Window::CreateDescriptorPool()
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

    void Window::SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height)
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

    // TODO: Figure out what most of this code does.
    void Window::RenderFrame(ImGui_ImplVulkanH_Window* wd, ImDrawData* drawData)
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
    void Window::RevealFrame(ImGui_ImplVulkanH_Window* wd)
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

    void Window::CleanupVulkan()
    {
        // Calls vkDestroyPipeline, vkDestroyRenderPass, vkDestroySwapchainKHR and vkDestroySurfaceKHR.
        ImGui_ImplVulkanH_DestroyWindow(_instance, _device, &_mainWindowData, _allocator);

        vkDestroyDescriptorPool(_device, _descriptorPool, _allocator);

        vkDestroyDevice(_device, _allocator);

        if (enableValidationLayers) vkb::destroy_debug_utils_messenger(_instance, _debugMessenger, _allocator);
        vkDestroyInstance(_instance, _allocator);
    }

    VkBool32 Window::DebugCallback(
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

    std::unique_ptr<Window> Window::Create(const WindowProperties& props)
    {
        return std::make_unique<Window>(props);
    }
}


