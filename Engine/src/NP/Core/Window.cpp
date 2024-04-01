#include "nppch.h"
#include "Window.h"

#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <VkBootstrap.h>

#include "Application.h"
#include "../../Platform/Vulkan/VulkanRenderer.h"

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
        _vulkanRenderer = std::make_unique<VulkanRenderer>(_windowContext);
        _vulkanRenderer->Setup();
    }

    void Window::OnUpdate(float deltaTime)
    {
        ProcessEvents(deltaTime);

        _vulkanRenderer->NewFrame(_windowData);

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
        ImGui::ColorEdit3("clear color", (float*)&_vulkanRenderer->ClearColor); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / _vulkanRenderer->GetFramerate(), _vulkanRenderer->GetFramerate());
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
        _vulkanRenderer->EndFrame();
    }

    void Window::ProcessEvents(float deltaTime)
    {
        InputManager* inputManager = &Application::Get().GetInputManager();
        inputManager->OnUpdate();

        const bool prevLostFocus = LostFocus();

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            const auto io = _vulkanRenderer->GetImGuiIO();

            _imGuiLostFocus = (io->WantCaptureKeyboard);

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
                    if(LostFocus() || event.key.repeat != 0) continue;
                    inputManager->ProcessKey(event.key.keysym.sym, event.type == SDL_KEYDOWN);
                continue;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    if(LostFocus()) continue;
                    inputManager->ProcessMouseButton(MOUSE_BUTTON_TO_SDL_MOUSE_BUTTON(event.button.button), event.type == SDL_MOUSEBUTTONDOWN, event.button.clicks == 2);
                continue;
                case SDL_MOUSEMOTION:
                    if(LostFocus()) continue;
                    inputManager->ProcessMouseMovement((float)event.motion.x/(float)_windowData.Width, (float)event.motion.y/(float)_windowData.Height, deltaTime);
                continue;
            }
        }

        if(!prevLostFocus && LostFocus())
        {
            inputManager->FlushInputs();
        }

        if(!LostFocus()) inputManager->PopulateKeyStates(SDL_GetKeyboardState(nullptr));
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
            case SDL_WINDOWEVENT_FOCUS_LOST:
                _windowLostFocus = true;
            break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                _windowLostFocus = false;
            break;

        }
    }

    void Window::Shutdown()
    {
        _vulkanRenderer->Cleanup();

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


