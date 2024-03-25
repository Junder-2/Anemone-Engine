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

        _windowContext->WindowCloseDelegate += MakeDelegate(this, &Application::Shutdown);
        _windowContext->WindowResizeDelegate += MakeDelegate(this, &Application::OnResize);
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

            _windowContext->OnUpdate();
        }
    }

    void Application::Shutdown()
    {
        _isRunning = false;
    }

    void Application::OnResize(int width, int height)
    {
        NP_ENGINE_LOG_INFO("new size {0}, {1}", width, height);
    }
}
