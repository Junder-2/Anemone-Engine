#include "anepch.h"
#include "InspectorPanel.h"

#include "imgui.h"

Engine::InspectorPanel::InspectorPanel()
{
}

Engine::InspectorPanel::InspectorPanel(EditorLayer* editorLayer)
{
    _EditorLayer = editorLayer;
}

Engine::InspectorPanel::~InspectorPanel()
{
}

void Engine::InspectorPanel::RegisterSelect(UUIDComponent selectedEntityID)
{
    selected = selectedEntityID.UUID;
}

void Engine::InspectorPanel::WipeSelect()
{
    selected = "";
}

void Engine::InspectorPanel::OnPanelRender()
{
    ImGui::Begin("Inspection");
    if(std::strcmp("",selected.c_str()))
        {
        ImGui::Text(selected.c_str());
        _EditorLayer->GetActiveScene();
        }
    else
        {
        ImGui::Text("Nothing selected");

        }

    ImGui::End();

    //ANE_LOG_INFO("Entering panel render phase");


}
