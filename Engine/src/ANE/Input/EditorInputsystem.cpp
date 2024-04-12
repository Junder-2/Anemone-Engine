#include "anepch.h"
#include "EditorInputsystem.h"

#include "ANE/Core/Application.h"
#include "ANE/Events/EventHandler.h"

Engine::EditorInputsystem::EditorInputsystem()
{
    _inputHandler = &Application::Get().GetInputHandler();

    EventHandler::BindEditorEvent(MakeDelegate(this, &EditorInputsystem::OnEvent));
}

void Engine::EditorInputsystem::OnEvent(Event& e)
{
    InputSystem::OnEvent(e);
}
