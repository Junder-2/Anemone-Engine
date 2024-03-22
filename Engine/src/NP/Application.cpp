#include "nppch.h"
#include "Application.h"

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
            for (Layer* layer : _layerStack)
            {
                layer->OnUpdate(0);
            }
            //todo frame yap

            _isRunning = _windowContext->OnUpdate();
        }
    }
}
