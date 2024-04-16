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
        ANE_PROFILE_FUNCTION();

        ANE_ELOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        // SDL
        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER))
        {
            ANE_ELOG_ERROR("Could not initialize SDL.");
        }

        const SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        _windowContext = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowData.Width, _windowData.Height, windowFlags);
        _windowData.Id = SDL_GetWindowID(_windowContext);

        // Set minimum size to prevent errors
        SDL_SetWindowMinimumSize(_windowContext, 180, 80);

        int xPos, yPos;
        SDL_GetWindowPosition(_windowContext, &xPos, &yPos);

        _windowData.XPos = xPos;
        _windowData.YPos = yPos;

        _viewports[_windowData.Id] = _windowData;
        _activeViewport = &_viewports[_windowData.Id];

        ANE_EASSERT(_windowContext, "Could not create SDL window.");
    }

    void Window::OnUpdate(float deltaTime)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        ProcessEvents(deltaTime);
    }

    void Window::ProcessEvents(float deltaTime)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        InputHandler* inputHandler = &Application::Get().GetInputHandler();
        inputHandler->OnUpdate();

        const bool prevHasFocus = HasFocus();
        const bool imGuiWantMouse = ImGui::GetIO().WantCaptureMouse;
        const bool relativeMouseMode = SDL_GetRelativeMouseMode();

        _imGuiHasFocus = ImGui::GetIO().WantCaptureKeyboard;

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            // While mouse is hidden we dont want to be able to interact with imgui
            if(!relativeMouseMode) ImGui_ImplSDL2_ProcessEvent(&event);

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
                    if(event.key.repeat != 0) continue;
                    inputHandler->ProcessKey(event.key.keysym.sym, event.type == SDL_KEYDOWN);
                }
                continue;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                {
                    if(imGuiWantMouse || _activeViewport->Id != event.button.windowID) continue;
                    const int keyIndex = MOUSE_BUTTON_TO_SDL_MOUSE_BUTTON(event.button.button);

                    inputHandler->ProcessMouseButton(keyIndex, event.type == SDL_MOUSEBUTTONDOWN, event.button.clicks == 2);
                }
                continue;
                case SDL_MOUSEWHEEL:
                {
                    if(imGuiWantMouse || _activeViewport->Id != event.wheel.windowID) continue;
                    const float x = event.wheel.preciseX;
                    const float y = event.wheel.preciseY;

                    inputHandler->ProcessMouseScroll(Vector2(x, y));
                }
                continue;
                case SDL_MOUSEMOTION:
                {
                    if(_activeViewport->Id != event.motion.windowID) return;

                    const ViewportProperties viewport = _viewports[event.motion.windowID];

                    const float viewportWidth = viewport.Width;
                    const float viewportHeight = viewport.Height;

                    const Vector2 absolutePos = GetAbsoluteMousePos();

                    // Convert mouse coord to relative
                    const float relX = std::clamp((float)event.motion.x/viewportWidth, 0.f, 1.f);
                    const float relY = std::clamp((float)event.motion.y/viewportHeight, 0.f, 1.f);
                    const float relDeltaX = ((float)event.motion.xrel/viewportWidth) * 60.f * deltaTime;
                    const float relDeltaY = ((float)event.motion.yrel/viewportHeight) * 60.f * deltaTime;

                    inputHandler->ProcessAbsoluteMouseMovement(absolutePos);
                    inputHandler->ProcessMouseMovement(Vector2(relX, relY), Vector2(relDeltaX, relDeltaY));
                }
            }
            continue;
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

            const Vector2 absolutePos = GetAbsoluteMousePos();

            int newX, newY;
            SDL_GetMouseState(&newX, &newY);
            const float x = std::clamp((float)newX/(float)_activeViewport->Width, 0.f, 1.f);
            const float y = std::clamp((float)newY/(float)_activeViewport->Height, 0.f, 1.f);

            inputHandler->ProcessAbsoluteMouseMovement(absolutePos);
            inputHandler->ProcessMouseMovement(Vector2(x, y), 0);
        }

        if(HasFocus()) inputHandler->PopulateKeyStates(SDL_GetKeyboardState(nullptr));
    }

    void Window::ProcessWindowEvent(const SDL_WindowEvent& windowEvent, float deltaTime)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        const bool isMainWindow = windowEvent.windowID == _windowData.Id;
        const bool isViewport = _viewports.contains(windowEvent.windowID);

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
                if(!isViewport) return;
                ViewportProperties& window = _viewports[windowEvent.windowID];

                const uint32_t newX = (uint32_t)windowEvent.data1;
                const uint32_t newY = (uint32_t)windowEvent.data2;
                if(window.Width == newX && window.Height == newY) break;

                window.Width = newX;
                window.Height = newY;

                if(!isMainWindow) return;

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
                if(!isViewport) return;
                ViewportProperties& window = _viewports[windowEvent.windowID];

                uint32_t newX = (uint32_t)windowEvent.data1;
                uint32_t newY = (uint32_t)windowEvent.data2;
                if(window.XPos == newX && window.YPos == newY) break;

                window.XPos = newX;
                window.YPos = newY;

                if(!isMainWindow) return;

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

    Vector2 Window::GetAbsoluteMousePos() const
    {
        int newX, newY;
        SDL_GetMouseState(&newX, &newY);
        newX += _windowData.XPos;
        newY += _windowData.YPos;
        const float x = std::clamp((float)newX/(float)_windowData.Width, 0.f, 1.f);
        const float y = std::clamp((float)newY/(float)_windowData.Height, 0.f, 1.f);

        return {x, y};
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

    void Window::SetActiveViewport(const uint32_t id)
    {
        if(!_viewports.contains(id)) return;

        _activeViewport = &_viewports[id];
    }

    void Window::SetActiveViewport(const ViewportProperties& props)
    {
        if(_viewports.contains(props.Id)) return;

        AddViewport(props);

        SetActiveViewport(props.Id);
    }

    void Window::AddViewport(const ViewportProperties& props)
    {
        if(_viewports.contains(props.Id)) return;

        _viewports[props.Id] = props;
    }

    void Window::RemoveViewport(const uint32_t id)
    {
        if(!_viewports.contains(id)) return;

        _viewports.erase(id);

        _activeViewport = &_viewports[_windowData.Id];
    }

    void Window::SetMouseVisibility(const bool enable)
    {
        SDL_SetRelativeMouseMode(enable ? SDL_TRUE : SDL_FALSE);
    }

    std::unique_ptr<Window> Window::Create(const WindowProperties& props)
    {
        return std::make_unique<Window>(props);
    }
}
