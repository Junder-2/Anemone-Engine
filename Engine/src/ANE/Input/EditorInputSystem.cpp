#include "anepch.h"
#include "EditorInputSystem.h"

#include "ANE/Core/Application.h"
#include "ANE/Events/EventHandler.h"

Engine::EditorInputSystem::EditorInputSystem()
{
    _inputHandler = &Application::Get().GetInputHandler();

    EventHandler::BindEditorEvent(MakeDelegate(this, &EditorInputSystem::OnEvent));
}

void Engine::EditorInputSystem::OnEvent(Event& e)
{
    InputSystem::OnEvent(e);
}
