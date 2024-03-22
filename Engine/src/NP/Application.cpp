#include "nppch.h"
#include "Application.h"

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
            
        }
    }
}
