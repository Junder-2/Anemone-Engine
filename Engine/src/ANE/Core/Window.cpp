#include "anepch.h"
#include "Window.h"

#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_internal.h>
#include <VkBootstrap.h>

#include "Application.h"
#include "ANE/Events/WindowEvent.h"
#include "ANE/Input/InputHandler.h"
#include "ANE/Math/FMath.h"
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

    void Window::BindOnEvent(const Delegate<void(Event&)>& delegate)
    {
        _eventDelegate = delegate;
    }

    void Window::Init(const WindowProperties& props)
    {
        ANE_PROFILE_FUNCTION();

        ANE_ELOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER))
        {
            ANE_ELOG_ERROR("Could not initialize SDL.");
        }

        constexpr SDL_WindowFlags windowFlags = static_cast<SDL_WindowFlags>(SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        const int width = static_cast<int>(_windowData.Width), height = static_cast<int>(_windowData.Height);
        _windowContext = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, windowFlags);
        _windowData.Id = SDL_GetWindowID(_windowContext);

        // Set minimum size to prevent errors
        SDL_SetWindowMinimumSize(_windowContext, 180, 80);

        int xPos, yPos;
        SDL_GetWindowPosition(_windowContext, &xPos, &yPos);

        _windowData.XPos = xPos;
        _windowData.YPos = yPos;

        SetActiveViewport(_windowData.Id);

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

        _imGuiHasFocus = ImGui::GetIO().WantCaptureKeyboard && !IsOverViewport();

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
                    const SDL_KeyboardEvent keyEvent = event.key;
                    if(keyEvent.repeat != 0) continue;
                    inputHandler->ProcessKey(keyEvent.keysym.sym, keyEvent.state == SDL_PRESSED);
                }
                continue;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                {
                    const SDL_MouseButtonEvent buttonEvent = event.button;

                    const int keyIndex = MOUSE_BUTTON_TO_SDL_MOUSE_BUTTON(buttonEvent.button);

                    inputHandler->ProcessMouseButton(keyIndex, buttonEvent.state == SDL_PRESSED, buttonEvent.clicks >= 2);
                }
                continue;
                case SDL_MOUSEWHEEL:
                {
                    const float x = event.wheel.preciseX;
                    const float y = event.wheel.preciseY;

                    inputHandler->ProcessMouseScroll(Vector2(x, y));
                }
                continue;
                case SDL_MOUSEMOTION:
                {
                    const ViewportProperties viewport = GetActiveViewportProperties();

                    const float viewportWidth = static_cast<float>(viewport.Width);
                    const float viewportHeight = static_cast<float>(viewport.Height);

                    const Vector2 absolutePos = GetAbsoluteMousePos();

                    // Convert mouse coord to relative
                    const float relX = FMath::Saturate(static_cast<float>(event.motion.x) / viewportWidth);
                    const float relY = FMath::Saturate(static_cast<float>(event.motion.y) / viewportHeight);
                    const float relDeltaX = (static_cast<float>(event.motion.xrel) / viewportWidth) * 60.f * deltaTime;
                    const float relDeltaY = (static_cast<float>(event.motion.yrel) / viewportHeight) * 60.f * deltaTime;

                    inputHandler->ProcessAbsoluteMouseMovement(absolutePos);
                    inputHandler->ProcessMouseMovement(Vector2(relX, relY), Vector2(relDeltaX, relDeltaY));
                }
                continue;
            }
        }

        if(!IsViewportMainWindow() && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            ProcessViewportEvents(_previousViewportProperties);

            _previousViewportProperties = GetActiveViewportProperties();
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

            const ViewportProperties viewport = GetActiveViewportProperties();

            int newX, newY;
            SDL_GetMouseState(&newX, &newY);

            const float x = FMath::Saturate(static_cast<float>(newX) / static_cast<float>(viewport.Width));
            const float y = FMath::Saturate(static_cast<float>(newY) / static_cast<float>(viewport.Height));

            inputHandler->ProcessAbsoluteMouseMovement(absolutePos);
            inputHandler->ProcessMouseMovement(Vector2(x, y), 0);
        }

        if(HasFocus()) inputHandler->PopulateKeyStates(SDL_GetKeyboardState(nullptr));
    }

    void Window::ProcessWindowEvent(const SDL_WindowEvent& windowEvent, float deltaTime)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        const bool isMainWindow = windowEvent.windowID == _windowData.Id;

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

                const uint32_t newX = static_cast<uint32_t>(windowEvent.data1);
                const uint32_t newY = static_cast<uint32_t>(windowEvent.data2);
                if(_windowData.Width == newX && _windowData.Height == newY) break;

                _windowData.Width = newX;
                _windowData.Height = newY;

                WindowResizeEvent resizeEvent(_windowData.Width, _windowData.Height, _windowData.Id);
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

                const uint32_t newX = static_cast<uint32_t>(windowEvent.data1);
                const uint32_t newY = static_cast<uint32_t>(windowEvent.data2);
                if(_windowData.XPos == newX && _windowData.YPos == newY) break;

                _windowData.XPos = newX;
                _windowData.YPos = newY;

                WindowMovedEvent movedEvent(_windowData.XPos, _windowData.YPos, _windowData.Id);
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

    void Window::ProcessViewportEvents(const ViewportProperties& previousProps)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        const ViewportProperties newViewportProps = GetActiveViewportProperties();
        if(newViewportProps.Width != previousProps.Width || newViewportProps.Height != previousProps.Height)
        {
            WindowResizeEvent resizeEvent(newViewportProps.Width, newViewportProps.Height, newViewportProps.Id);
            DispatchEvent(resizeEvent);
        }

        if(newViewportProps.XPos != previousProps.XPos || newViewportProps.YPos != previousProps.YPos)
        {
            WindowMovedEvent movedEvent(newViewportProps.XPos, newViewportProps.YPos, newViewportProps.Id);
            DispatchEvent(movedEvent);
        }
    }

    Vector2 Window::GetAbsoluteMousePos() const
    {
        ANE_DEEP_PROFILE_FUNCTION();

        int newX, newY;
        SDL_GetMouseState(&newX, &newY);
        newX += static_cast<int>(_windowData.XPos);
        newY += static_cast<int>(_windowData.YPos);
        const float x = FMath::Saturate(static_cast<float>(newX) / static_cast<float>(_windowData.Width));
        const float y = FMath::Saturate(static_cast<float>(newY) / static_cast<float>(_windowData.Height));

        return {x, y};
    }

    void Window::DispatchEvent(Event& e)
    {
        ANE_PROFILE_FUNCTION();

        if(_eventDelegate) _eventDelegate(e);
    }

    void Window::Shutdown()
    {
        ANE_PROFILE_FUNCTION();

        SDL_DestroyWindow(_windowContext);
        SDL_Quit();
    }

    // Not implemented.
    void Window::SetVSync(const bool enabled)
    {
        _windowData.VSync = enabled;
    }

    ViewportProperties Window::GetActiveViewportProperties() const
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if(IsViewportMainWindow())
        {
            return ViewportProperties(_windowData.Id, _windowData.Width, _windowData.Height);
        }

        const ImGuiWindow* imGuiWindow = ImGui::FindWindowByID(_activeViewportId);
        const uint32_t width = static_cast<uint32_t>(FMath::Max(imGuiWindow->ContentSize.x, 1.f)), height = static_cast<uint32_t>(FMath::Max(imGuiWindow->ContentSize.y, 1.f));
        const uint32_t xPos = static_cast<uint32_t>(imGuiWindow->Pos.x), yPos = static_cast<uint32_t>(imGuiWindow->Pos.y);
        return ViewportProperties(_activeViewportId, width, height, xPos, yPos);
    }

    void Window::SetActiveViewport(const uint32_t id)
    {
        ANE_PROFILE_FUNCTION();

        if(_activeViewportId == id) return;
        AddViewport(id);
        _previousViewportId = _activeViewportId;
        _activeViewportId = id;

        ProcessViewportEvents(_previousViewportProperties);
    }

    void Window::AddViewport(uint32_t id)
    {
        ANE_PROFILE_FUNCTION();

        if(_viewports.contains(id)) return;

        _viewports.emplace(id);
    }

    void Window::RemoveViewport(const uint32_t id)
    {
        ANE_PROFILE_FUNCTION();

        if(!_viewports.contains(id)) return;

        _viewports.erase(id);

        _activeViewportId = _previousViewportId;
    }

    bool Window::IsOverViewport() const
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if(_activeViewportId == _windowData.Id) return !ImGui::GetIO().WantCaptureMouse;
        const auto& rect = ImGui::FindWindowByID(_activeViewportId)->InnerRect;

        return ImGui::IsMouseHoveringRect(rect.Min, rect.Max, false);
    }

    void Window::SetMouseVisibility(const bool enable)
    {
        ANE_PROFILE_FUNCTION();

        // TODO: This seems to unfocus out of the window
        SDL_SetRelativeMouseMode(enable ? SDL_FALSE : SDL_TRUE);
        SDL_ShowCursor(enable);

        if(enable)
        {
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        }
        else
        {
            ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
            ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
        }
    }

    bool Window::IsMouseVisible()
    {
        return !SDL_GetRelativeMouseMode();
    }

    std::unique_ptr<Window> Window::Create(const WindowProperties& props)
    {
        return std::make_unique<Window>(props);
    }
}
