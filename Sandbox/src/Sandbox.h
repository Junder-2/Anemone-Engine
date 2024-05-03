#pragma once

#include <..\..\Engine\src\ANE.h>
#include "UI/SandboxLayer.h"

class Sandbox : public Engine::Application
{
public:
    Sandbox(const Engine::ApplicationSpecification& specification) : Application(specification)
    {
        SandBox::SandBoxLayer* sandBoxLayer = new SandBox::SandBoxLayer();
        PushLayer(sandBoxLayer);
    }
    ~Sandbox() { }
};
