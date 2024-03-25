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

        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER))
        {
            NP_ENGINE_LOG_ERROR("Could not init sdl");
        }

        SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        _windowContext = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowData.Width, _windowData.Height, windowFlags);
        if (_windowContext == nullptr)
        {
            NP_ENGINE_LOG_ERROR("Could not create SDL window.");
        }

        _renderer = SDL_CreateRenderer(_windowContext, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
        if (_renderer == nullptr)
        {
            NP_ENGINE_LOG_ERROR("Could not create SDL renderer.");
        }

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
        ImGui_ImplSDL2_InitForSDLRenderer(_windowContext, _renderer);
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
        SDL_RenderSetScale(_renderer, _io->DisplayFramebufferScale.x, _io->DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(_renderer, (Uint8)(_clearColor.x * 255), (Uint8)(_clearColor.y * 255), (Uint8)(_clearColor.z * 255), (Uint8)(_clearColor.w * 255));
        SDL_RenderClear(_renderer);
        SDL_RenderPresent(_renderer);
    }

    void Window::Shutdown()
    {
        // Cleanup
        const VkResult err = vkDeviceWaitIdle(_device);
        CheckVKResult(err);
        CleanupImGui();

        CleanupVulkanWindow();
        CleanupVulkan();

        SDL_DestroyRenderer(_renderer);
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

    void Window::CleanupVulkanWindow()
    {
        //ImGui_ImplVulkanH_DestroyWindow(g_Instance, _device, &g_MainWindowData, g_Allocator);
    }

    void Window::CleanupVulkan()
    {
        //vkDestroyDescriptorPool(_device, g_DescriptorPool, g_Allocator);

        //vkDestroyDevice(_device, g_Allocator);
        //vkDestroyInstance(g_Instance, g_Allocator);
    }

    std::unique_ptr<Window> Window::Create(const WindowProperties& props)
    {
        return std::make_unique<Window>(props);
    }
}


