#include "nppch.h"
#include "Application.h"

#include <SDL_timer.h>

#include "Layer.h"

namespace Engine
{
    Application* Application::_appInstance = nullptr;

    Application::Application(const ApplicationSpecification& specification) : _appSpec(specification)
    {
        _appInstance = this;

        _windowContext = Window::Create(WindowProperties(_appSpec.Name));
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

            _isRunning = _windowContext->OnUpdate();
        }
    }
}
