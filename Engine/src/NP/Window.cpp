#include "nppch.h"
#include "Window.h"

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_vulkan.h>

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
        const ImVector<const char*> extensions = GetAvailableExtensions(_windowContext);
        CreateVulkanInstance(extensions);

        VkSurfaceKHR surface;
        if (SDL_Vulkan_CreateSurface(_windowContext, g_Instance, &surface) == 0)
        {
            NP_ENGINE_LOG_ERROR("Could not create Vulkan surface.\n");
        }

        int w, h;
        SDL_GetWindowSize(_windowContext, &w, &h);
        ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
        SetupVulkanWindow(wd, surface, w, h);

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
        ImGui_ImplSDL2_InitForVulkan(_windowContext);
        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = g_Instance;
        initInfo.PhysicalDevice = g_PhysicalDevice;
        initInfo.Device = g_Device;
        initInfo.QueueFamily = g_QueueFamily;
        initInfo.Queue = g_Queue;
        initInfo.PipelineCache = g_PipelineCache;
        initInfo.DescriptorPool = g_DescriptorPool;
        initInfo.RenderPass = wd->RenderPass;
        initInfo.Subpass = 0;
        initInfo.MinImageCount = g_MinImageCount;
        initInfo.ImageCount = wd->ImageCount;
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        initInfo.Allocator = g_Allocator;
        initInfo.CheckVkResultFn = CheckVKResult;
        ImGui_ImplVulkan_Init(&initInfo);
    }

    void Window::OnUpdate()
    {
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
                const auto windowEvent = event.window;
                if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    _windowData.Width = windowEvent.data1;
                    _windowData.Height = windowEvent.data2;

                    if(WindowResizeDelegate) WindowResizeDelegate(_windowData.Width, _windowData.Height);
                }
            }
        }

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
    }

    void Window::Shutdown()
    {
        // Cleanup
        const VkResult err = vkDeviceWaitIdle(g_Device);
        CheckVKResult(err);
        CleanupImGui();

        CleanupVulkanWindow();
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
    void Window::CheckVKResult(VkResult err)
    {
        if (err == 0)
            return;
        NP_ENGINE_LOG_ERROR("Vulkan Error: VkResult = {0}", (int)err);
        if (err < 0)
            abort();
    }

    ImVector<const char*> Window::GetAvailableExtensions(SDL_Window* window)
    {
        uint32_t extensionCount = 0;
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
        ImVector<const char*> extensions;
        extensions.resize(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.Data);
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

    void Window::CreateVulkanInstance(const ImVector<const char*>& extensions)
    {
        VkResult err;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        uint32_t propertiesCount;
        vkEnumerateInstanceExtensionProperties(nullptr, &propertiesCount, nullptr);
        ImVector<VkExtensionProperties> properties;
        properties.resize(propertiesCount);
        err = vkEnumerateInstanceExtensionProperties(nullptr, &propertiesCount, properties.Data);
        CheckVKResult(err);

        // Validation layers
        createInfo.enabledLayerCount = 0;
        if (enableValidationLayers)
        {
            if (CheckValidationLayers())
            {
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();
            }
            else
            {
                NP_ENGINE_LOG_WARN("Vulkan Error: Creating instance without validation layers since one or more are unavailable.");
            }
        }

        createInfo.enabledExtensionCount = (uint32_t)extensions.Size;
        createInfo.ppEnabledExtensionNames = extensions.Data;
        err = vkCreateInstance(&createInfo, g_Allocator, &g_Instance);
        CheckVKResult(err);
    }

    void Window::SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height)
    {
        wd->Surface = surface;

        // Check for WSI support
        VkBool32 res;
        vkGetPhysicalDeviceSurfaceSupportKHR(g_PhysicalDevice, g_QueueFamily, wd->Surface, &res);
        if (res != VK_TRUE)
        {
            NP_ENGINE_LOG_ERROR("Error no WSI support on physical device 0\n");
        }

        // Select Surface Format
        constexpr VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
        constexpr VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(g_PhysicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

        // Select Present Mode
        #ifdef APP_UNLIMITED_FRAME_RATE
            VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
        #else
            VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
        #endif
        wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(g_PhysicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
        //NP_ENGINE_LOG_INFO("Vulkan Info: Selected PresentMode = {0}", wd->PresentMode);

        // Create SwapChain, RenderPass, Framebuffer, etc.
        IM_ASSERT(g_MinImageCount >= 2);
        ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, wd, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
    }

    void Window::CleanupVulkanWindow()
    {
        ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &g_MainWindowData, g_Allocator);
    }

    void Window::CleanupVulkan()
    {
        vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);

        vkDestroyDevice(g_Device, g_Allocator);
        vkDestroyInstance(g_Instance, g_Allocator);
    }

    std::unique_ptr<Window> Window::Create(const WindowProperties& props)
    {
        return std::make_unique<Window>(props);
    }
}


