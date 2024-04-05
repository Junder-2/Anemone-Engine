﻿#include "anepch.h"
#include "EditorLayer.h"
#include <imgui.h>

#include "ANE/Core/Entity/Entity.h"
#include "LayerStack.h"

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

}

void Engine::EditorLayer::OnUIRender()
{
    ImGui::Begin("Hello");
    ImGui::Button("Button");
    ImGui::End();

    ImGui::ShowDemoWindow();
}

void Engine::EditorLayer::OnUpdate(float deltaTime)
{
    // // is the
    //
    // //if playmode
    // if (_activeScene)
    //     _activeScene->OnUpdate(deltaTime);

}
