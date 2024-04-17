#pragma once

#include <..\..\Engine\src\ANE.h>

#include "ANE/Core/Layers/EditorLayer.h"
#include "ANE/Core/Layers/ImGuiLayer.h"

class Editor : public Engine::Application
{
public:
    Editor(const Engine::ApplicationSpecification& specification) : Application(specification)
    {

        Engine::EditorLayer* editorLayer = new Engine::EditorLayer("EditorLayer");
        PushLayer(editorLayer);
    }
    ~Editor() { }
};
