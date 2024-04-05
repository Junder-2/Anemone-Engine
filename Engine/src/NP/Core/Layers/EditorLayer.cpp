#include "nppch.h"
#include "EditorLayer.h"
#include "LayerStack.h"

Engine::EditorLayer::~EditorLayer()
{
    //LayerStack stacky;
    //stacky::PushLayer(this);
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
