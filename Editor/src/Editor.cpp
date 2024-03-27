#include "Editor.h"

Engine::Application* Engine::CreateApplication()
{
    ApplicationSpecification spec;
    spec.Name = "NP Editor";
    spec.LogName = "NP EDITOR";

    return new Editor(spec);
}
