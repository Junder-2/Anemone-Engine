#include "nppch.h"
#include "Application.h"

#include "Layer.h"

namespace Engine
{
    Application* Application::appInstance = nullptr;

    Application::Application(const ApplicationSpecification& specification) : appSpec(specification)
    {
        appInstance = this;
    }

    Application::~Application() = default;

    void Application::Run()
    {
        while (isRunning)
        {
            for (Layer* layer : _layerStack)
            {
                layer->OnUpdate(0);
            }
        }
    }
}
