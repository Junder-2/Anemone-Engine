#include "nppch.h"
#include "Window.h"

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

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

        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
        {
            NP_ENGINE_LOG_ERROR("Could not init sdl");
        }

        SDL_Vulkan_LoadLibrary(nullptr);
        SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        _windowContext = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowData.Width, _windowData.Height, windowFlags);
        if (_windowContext == nullptr)
        {
            NP_ENGINE_LOG_ERROR("Could not create SDL window.");
        }

        SDL_Renderer* renderer = SDL_CreateRenderer(_windowContext, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr)
        {
            SDL_Log("Could not create SDL renderer!");
        }

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForSDLRenderer(_windowContext, renderer);
        ImGui_ImplSDLRenderer2_Init(renderer);
    }

    bool Window::OnUpdate()
    {
        bool shouldQuit = false;

        SDL_Event windowEvent;
        while(SDL_PollEvent(&windowEvent))
        {
            switch (windowEvent.type)
            {
            case SDL_QUIT:
                shouldQuit = true;
                break;
            }
        }

        return !shouldQuit;
    }

    void Window::Shutdown()
    {
        vkDestroyDevice(_vkDevice, nullptr);
        vkDestroyInstance(_vkInstance, nullptr);
        SDL_Vulkan_UnloadLibrary();
        SDL_DestroyWindow(_windowContext);
        SDL_Quit();
    }

    void Window::SetVSync(const bool enabled)
    {
        //todo
        _windowData.VSync = enabled;
    }

    std::unique_ptr<Window> Window::Create(const WindowProperties& props)
    {
        return std::make_unique<Window>(props);
    }
}


