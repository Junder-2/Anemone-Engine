#include "Editor.h"

std::string Engine::GetApplicationName()
{
    return "Anemone Editor";
}

Engine::Application* Engine::CreateApplication()
{
    ApplicationSpecification spec;
    spec.Name = GetApplicationName();
    spec.LogName = "ANE EDITOR";

    return new Editor(spec);
}
