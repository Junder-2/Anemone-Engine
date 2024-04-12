#include "anepch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

Engine::ImGuiLayer::ImGuiLayer(const char* layerName): Layer(layerName)//, _flags(ImGuiUtilities::ImGuiDockNodeFlags_None)
{
    _flags = static_cast<ImGuiUtilities::ImGuiDockNodeFlags>(_flags | ImGuiDockNodeFlags_KeepAliveOnly);
    // ImGuiID ID = ImGui::GetID("DemoDsockSpace");
    // ImGui::id
   // ImGui::DockSpace(ID, ImVec2(100, 100), _flags);
}

Engine::ImGuiLayer::~ImGuiLayer()
{
}

void Engine::ImGuiLayer::OnAttach()
{
}

void Engine::ImGuiLayer::OnDetach()
{


}

void Engine::ImGuiLayer::OnUpdate(float deltaTime)
{
    //1

}

void Engine::ImGuiLayer::OnEvent(Event& e)
{

}

void Engine::ImGuiLayer::OnUIRender()
{
    //2
    ImGui::Begin("We have a window");

    ImGui::End();

}
