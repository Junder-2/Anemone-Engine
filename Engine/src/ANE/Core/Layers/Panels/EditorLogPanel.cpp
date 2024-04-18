#include "anepch.h"
#include "EditorLogPanel.h"

#include "imgui.h"


Engine::EditorLogPanel::EditorLogPanel()
{

}

Engine::EditorLogPanel::~EditorLogPanel()
{

}


void Engine::EditorLogPanel::OnPanelRender()
{
    ImGui::Begin("Log Window");
    ImGui::End();
}
