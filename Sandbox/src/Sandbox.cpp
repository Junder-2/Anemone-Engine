#include "Sandbox.h"

std::string Engine::GetApplicationName()
{
    return "Sandbox";
}

Engine::Application* Engine::CreateApplication()
{
    ApplicationSpecification spec;
    spec.Name = GetApplicationName();

    return new Sandbox(spec);
}
