﻿#include "anepch.h"
#include "EditorLayer.h"
#include "ANE/Core/Entity/Entity.h"
#include "LayerStack.h"
#include "ANE/Events/EventHandler.h"

Engine::EditorLayer::EditorLayer(const std::string& name) : Layer(name)
{
}

Engine::EditorLayer::~EditorLayer()
{


}

void Engine::EditorLayer::OnAttach()
{
}

void Engine::EditorLayer::OnEvent(Event& e)
{
    EventHandler::DispatchEditorEvents();
    EventHandler::DispatchAppEvents();
}

void Engine::EditorLayer::OnUpdate(float deltaTime)
{
    if (_activeScene) _activeScene->OnUpdate(deltaTime);
}

void Engine::EditorLayer::OnEventTest(Event& e)
{
    ANE_LOG_INFO("Hello {0}?: {1}", _debugName, e.GetEventCategories());
}

