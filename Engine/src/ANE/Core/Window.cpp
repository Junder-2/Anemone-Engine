#include "anepch.h"
#include "Window.h"

#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <VkBootstrap.h>

#include "Application.h"
#include "ANE/Input/Input.h"
#include "ANE/Utilities/InputUtilities.h"
#include "Platform/Vulkan/VulkanRenderer.h"

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
        ANE_ELOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        // SDL
        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER))
        {
            ANE_ELOG_ERROR("Could not initialize SDL.");
        }

        const SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        _windowContext = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowData.Width, _windowData.Height, windowFlags);


        int xPos, yPos;
        SDL_GetWindowPosition(_windowContext, &xPos, &yPos);

        _windowData.XPos = xPos;
        _windowData.YPos = yPos;

        if (_windowContext == nullptr)
        {
            ANE_ELOG_ERROR("Could not create SDL window.");
        }
    }

    void Window::OnUpdate(float deltaTime)
    {
        ProcessEvents(deltaTime);

        return;

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
        //ImGui::ColorEdit3("clear color", (float*)&_vulkanRenderer->ClearColor); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / _vulkanRenderer->GetFramerate(), _vulkanRenderer->GetFramerate());
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
        //_vulkanRenderer->EndFrame();
    }

    void Window::ProcessEvents(float deltaTime)
    {
        InputHandler* inputHandler = &Application::Get().GetInputHandler();
        inputHandler->OnUpdate();

        const bool prevHasFocus = HasFocus();
        const bool blockInputs = !HasFocus();
        const bool imGuiWantMouse = ImGui::GetIO().WantCaptureMouse;

        _imGuiHasFocus = ImGui::GetIO().WantCaptureKeyboard;

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            switch (event.type)
            {
                case SDL_QUIT:
                {
                    WindowCloseEvent closeEvent;
                    DispatchEvent(closeEvent);
                }
                break;
                case SDL_WINDOWEVENT:
                    ProcessWindowEvent(event.window, deltaTime);
                continue;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                {
                    if(blockInputs || event.key.repeat != 0) continue;
                    inputHandler->ProcessKey(event.key.keysym.sym, event.type == SDL_KEYDOWN);
                }
                continue;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                {
                    if(blockInputs || imGuiWantMouse) continue;
                    const int keyIndex = MOUSE_BUTTON_TO_SDL_MOUSE_BUTTON(event.button.button);

                    inputHandler->ProcessMouseButton(keyIndex, event.type == SDL_MOUSEBUTTONDOWN, event.button.clicks == 2);
                }
                continue;
                case SDL_MOUSEWHEEL:
                {
                    if(blockInputs) continue;
                    const float x = event.wheel.preciseX;
                    const float y = event.wheel.preciseY;

                    inputHandler->ProcessMouseScroll(x, y);
                }
                continue;
                case SDL_MOUSEMOTION:
                {
                    if(blockInputs) continue;

                    const float x = std::clamp((float)event.motion.x/(float)_windowData.Width, 0.f, 1.f);
                    const float y = std::clamp((float)event.motion.y/(float)_windowData.Height, 0.f, 1.f);

                    inputHandler->ProcessMouseMovement(x, y, deltaTime);
                }
                continue;
            }
        }

        if(prevHasFocus && !HasFocus())
        {
            WindowFocusChangeEvent focusChangeEvent(false);
            DispatchEvent(focusChangeEvent);
            inputHandler->FlushInputs();
        }
        else if(!prevHasFocus && HasFocus())
        {
            WindowFocusChangeEvent focusChangeEvent(true);
            DispatchEvent(focusChangeEvent);
            int newX, newY;
            SDL_GetMouseState(&newX, &newY);
            const float x = std::clamp((float)newX/(float)_windowData.Width, 0.f, 1.f);
            const float y = std::clamp((float)newY/(float)_windowData.Height, 0.f, 1.f);

            inputHandler->ProcessMouseMovement(x, y, 0);
        }

        if(HasFocus()) inputHandler->PopulateKeyStates(SDL_GetKeyboardState(nullptr));
    }

    void Window::ProcessWindowEvent(const SDL_WindowEvent& windowEvent, float deltaTime)
    {
        const bool isMainWindow = windowEvent.windowID == SDL_GetWindowID(_windowContext);
        switch (windowEvent.event)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if(!isMainWindow) return;
                WindowCloseEvent closeEvent;
                DispatchEvent(closeEvent);
            }
            break;
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            {
                if(!isMainWindow) return;
                const uint32_t newX = (uint32_t)windowEvent.data1;
                const uint32_t newY = (uint32_t)windowEvent.data2;
                if(_windowData.Width == newX && _windowData.Height == newY) break;

                _windowData.Width = newX;
                _windowData.Height = newY;

                WindowResizeEvent resizeEvent(_windowData.Width, _windowData.Height);
                DispatchEvent(resizeEvent);
            }
            break;
            case SDL_WINDOWEVENT_MINIMIZED:
            case SDL_WINDOWEVENT_RESTORED:
            case SDL_WINDOWEVENT_MAXIMIZED:
            {
                if(!isMainWindow) return;

                const WindowState windowState = windowEvent.event == SDL_WINDOWEVENT_MAXIMIZED ? WindowMaximized
                : windowEvent.event == SDL_WINDOWEVENT_RESTORED ? WindowRestored : WindowMinimized;

                WindowStateChangeEvent stateChangeEvent(windowState);
                DispatchEvent(stateChangeEvent);
            }
            break;
            case SDL_WINDOWEVENT_MOVED:
            {
                if(!isMainWindow) return;
                uint32_t newX = (uint32_t)windowEvent.data1;
                uint32_t newY = (uint32_t)windowEvent.data2;
                if(_windowData.XPos == newX && _windowData.YPos == newY) break;

                const float xDelta = ((float)newX - (float)_windowData.XPos) * deltaTime;
                const float yDelta = ((float)newY - (float)_windowData.YPos) * deltaTime;
                _windowData.XPos = newX;
                _windowData.YPos = newY;

                WindowMovedEvent movedEvent(_windowData.XPos, _windowData.YPos, xDelta, yDelta);
                DispatchEvent(movedEvent);
            }
            break;
            case SDL_WINDOWEVENT_ENTER:
                _windowHasFocus = isMainWindow;
            break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                _windowHasFocus = false;
            break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                _windowHasFocus = true;
            break;
        }
    }

    void Window::DispatchEvent(Event& e)
    {
        if(_eventDelegate) _eventDelegate(e);
    }

    void Window::Shutdown()
    {
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
