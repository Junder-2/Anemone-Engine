#pragma once

#include "Sandbox.h"


std::string Engine::GetApplicationName()
{
    return "Sandbox";
}

Engine::Application* Engine::CreateApplication()
{
    ApplicationSpecification spec;
    spec.Name = GetApplicationName();
    spec.LogName = "SANDBOX";

    return new Sandbox(spec);
}
