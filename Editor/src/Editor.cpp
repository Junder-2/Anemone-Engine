#include "Editor.h"

Engine::Application* Engine::CreateApplication()
{
    ApplicationSpecification spec;
    spec.Name = "Anemone Editor";
    spec.LogName = "ANE EDITOR";

    return new Editor(spec);
}
