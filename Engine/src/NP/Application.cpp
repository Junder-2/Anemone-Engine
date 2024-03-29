#include "nppch.h"
#include "Application.h"

#include <bitset>
#include <SDL_keycode.h>
#include <SDL_timer.h>

#include "Layer.h"
#include "Input/InputManager.h"

namespace Engine
{
    Application* Application::_appInstance = nullptr;

    Application::Application(const ApplicationSpecification& specification) : _appSpec(specification)
    {
        _appInstance = this;

        _windowContext = Window::Create(WindowProperties(_appSpec.Name));
        _inputManager = InputManager::Create();

        _windowContext->WindowCloseDelegate += MakeDelegate(this, &Application::Shutdown);
        _windowContext->WindowResizeDelegate += MakeDelegate(this, &Application::OnResizeTest);

        // input testing
        // GetInputManager().RegisterKeyboardTrigger(SDLK_o);
        // GetInputManager().BindKeyboardTrigger(SDLK_o, MakeDelegate(this, &Application::OnKeyTest));
        // GetInputManager().BindMouseButtonAction(MakeDelegate(this, &Application::OnMouseKeyTest));
        // GetInputManager().BindMouseMoveAction(MakeDelegate(this, &Application::OnMouseMoveTest));
    }

    Application::~Application() = default;

    void Application::Run()
    {
        while (_isRunning)
        {
            const Uint64 timeStamp = SDL_GetPerformanceCounter();
            const Uint64 timeStep = timeStamp - _lastTimeStamp;
            _lastTimeStamp = timeStamp;

            float deltaTime = timeStep / static_cast<float>(SDL_GetPerformanceFrequency());

            for (Layer* layer : _layerStack)
            {
                layer->OnUpdate(deltaTime);
            }
            //todo frame yap

            _windowContext->OnUpdate(deltaTime);

            // auto pressed = GetInputManager().GetKeyTriggerState(SDLK_o);
            //
            // if(pressed != TriggerNone)
            // {
            //     NP_ENGINE_LOG_INFO("O key state {0}", (int)pressed);
            // }
        }
    }

    void Application::Shutdown()
    {
        _isRunning = false;
    }

    void Application::OnResizeTest(int width, int height)
    {
        NP_ENGINE_LOG_INFO("new size {0}, {1}", width, height);
    }

    void Application::OnKeyTest(InputValue inputValue)
    {
        NP_ENGINE_LOG_INFO("pressed O {0}", inputValue.GetIntValue());
    }

    void Application::OnMouseKeyTest(MouseButtonValue inputValue)
    {
        NP_ENGINE_LOG_INFO("pressed mouse key {0}, with state {1}, is doubleclick {2}", inputValue.GetCurrentButtonIndex(), (int)inputValue.GetTriggerState(), inputValue.GetIsDoubleClick());
        NP_ENGINE_LOG_INFO("Raw mouse button state {0}", std::bitset<16>(inputValue.GetRawButtonStates()).to_string());
    }

    void Application::OnMouseMoveTest(MouseMoveValue inputValue)
    {
        // spdlog formatting not working use explicit glm::to_string
        NP_ENGINE_LOG_TRACE("moved mouse pos:({0}), delta:({1})", glm::to_string(inputValue.GetMousePos()), glm::to_string(inputValue.GetMouseDelta()));
    }
}
