#include "nppch.h"
#include "Application.h"

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
        NP_ENGINE_LOG_INFO("pressed mouse key {0}, with state {1}", inputValue.GetCurrentButtonIndex(), (int)inputValue.GetTriggerState());
    }

    void Application::OnMouseMoveTest(MouseMoveValue inputValue)
    {
        auto pos = inputValue.GetMousePos();
        auto delta = inputValue.GetMouseDelta();
        NP_ENGINE_LOG_INFO("moved mouse pos:({0}, {1}), delta:({2}, {3})", pos.x, pos.y, delta.x, delta.y);
    }
}
