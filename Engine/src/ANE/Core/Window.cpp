#include "anepch.h"
#include "Window.h"

#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <VkBootstrap.h>

#include "Application.h"
#include "../../Platform/Vulkan/VulkanRenderer.h"
#include "../Events/WindowEvent.h"

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
        ANE_ENGINE_LOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        // SDL
        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER))
        {
            ANE_ENGINE_LOG_ERROR("Could not initialize SDL.");
        }

        SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        _windowContext = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowData.Width, _windowData.Height, windowFlags);

        int xPos, yPos;
        SDL_GetWindowPosition(_windowContext, &xPos, &yPos);

        _windowData.XPos = xPos;
        _windowData.YPos = yPos;

        if (_windowContext == nullptr)
        {
            ANE_ENGINE_LOG_ERROR("Could not create SDL window.");
        }

        // Vulkan
        //_vulkanRenderer = std::make_unique<VulkanRenderer>(_windowContext);
        //_vulkanRenderer->Setup();
    }

    void Window::OnUpdate(float deltaTime)
    {
        ProcessEvents(deltaTime);

        //_vulkanRenderer->NewFrame(_windowData);

        return;
        static bool showSimpleOverlay = true;
        if (showSimpleOverlay) ShowInputDebugOverlay(&showSimpleOverlay);

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
        InputManager* inputManager = &Application::Get().GetInputManager();
        inputManager->OnUpdate();

        const bool prevLostFocus = LostFocus();

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            //const auto io = _vulkanRenderer->GetImGuiIO();

            //_imGuiLostFocus = (io->WantCaptureKeyboard);

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
                    if(LostFocus() || event.key.repeat != 0) continue;
                    inputManager->ProcessKey(event.key.keysym.sym, event.type == SDL_KEYDOWN);
                }
                continue;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                {
                    if(LostFocus()) continue;
                    const int keyIndex = MOUSE_BUTTON_TO_SDL_MOUSE_BUTTON(event.button.button);

                    inputManager->ProcessMouseButton(keyIndex, event.type == SDL_MOUSEBUTTONDOWN, event.button.clicks == 2);
                }
                continue;
                case SDL_MOUSEWHEEL:
                {
                    if(LostFocus()) continue;
                    const float x = event.wheel.preciseX;
                    const float y = event.wheel.preciseY;

                    inputManager->ProcessMouseScroll(x, y);
                }
                continue;
                case SDL_MOUSEMOTION:
                {
                    if(LostFocus()) continue;

                    const float x = std::clamp((float)event.motion.x/(float)_windowData.Width, 0.f, 1.f);
                    const float y = std::clamp((float)event.motion.y/(float)_windowData.Height, 0.f, 1.f);

                    inputManager->ProcessMouseMovement(x, y, deltaTime);
                }
                continue;
            }
        }

        if(!prevLostFocus && LostFocus())
        {
            WindowFocusChangeEvent focusChangeEvent(false);
            DispatchEvent(focusChangeEvent);
            inputManager->FlushInputs();
        }
        else if(prevLostFocus && !LostFocus())
        {
            WindowFocusChangeEvent focusChangeEvent(true);
            DispatchEvent(focusChangeEvent);
        }

        if(!LostFocus()) inputManager->PopulateKeyStates(SDL_GetKeyboardState(nullptr));
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

                const float xDelta = ((int)_windowData.XPos- (int)newX) * deltaTime;
                const float yDelta = ((int)_windowData.YPos - (int)newY) * deltaTime;
                _windowData.XPos = newX;
                _windowData.YPos = newY;

                WindowMovedEvent movedEvent(_windowData.XPos, _windowData.YPos, xDelta, yDelta);
                DispatchEvent(movedEvent);
            }
            break;
            case SDL_WINDOWEVENT_ENTER:
                _windowLostFocus = !isMainWindow;
            break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                _windowLostFocus = true;
            break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                _windowLostFocus = false;
            break;
        }
    }

    void Window::DispatchEvent(Event& e)
    {
        if(EventDelegate) EventDelegate(e);
    }

    void Window::ShowInputDebugOverlay(bool* pOpen)
    {
        InputManager* inputManager = &Application::Get().GetInputManager();
        static int location = 0;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDocking;
        if (location >= 0)
        {
            const float PAD = 10.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Example: Simple overlay", pOpen, window_flags))
        {
            const glm::vec2 mousePos = inputManager->GetMousePos();
            ImGui::Text("Mouse Pos: (%.3f,%.3f)", mousePos.x, mousePos.y);
            const MouseButtonValues mouseButtonValues = inputManager->GetMouseButtonValues();
            ImGui::Text("Mouse Buttons: (");
            bool start = true;
            for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
            {
                const int buttonState = mouseButtonValues.GetTriggerState(i);
                if (buttonState == 0) continue;
                ImGui::SameLine(0, start ? .0f : -1.0f);
                ImGui::Text("%d:%d", i, buttonState);
                start = false;
            }
            ImGui::SameLine(0, 0);
            ImGui::Text(")");

            const auto keyValues = inputManager->GetCurrentTriggeredKeys();
            ImGui::Text("Keyboard: (");
            start = true;
            for (auto keyValue : keyValues)
            {
                if(keyValue.GetIntValue() == 0) continue;
                ImGui::SameLine(0, start ? .0f : -1.0f);
                ImGui::Text("%d:%d", keyValue.GetBindingId(), keyValue.GetIntValue());
                start = false;
            }
            ImGui::SameLine(0, 0);
            ImGui::Text(")");

            if (ImGui::BeginPopupContextWindow())
            {
                if (pOpen && ImGui::MenuItem("Close")) *pOpen = false;
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    void Window::Shutdown()
    {
        //_vulkanRenderer->Cleanup();

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


