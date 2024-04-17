#include "anepch.h"
#include "InspectorPanel.h"

#include "imgui.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Core/Scene/Components/RenderComponent.h"

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
    std::vector<std::string>* selectedEntityUUIDS = SelectionManager::GetSelection(SelectionManager::UI);
    if(selectedEntityUUIDS->size() > 0){
        const char* name = "Untagged entity";
        Entity selectedEntity = _EditorLayer->GetActiveScene()->GetEntityWithUUID(selectedEntityUUIDS->at(0));
        TagComponent Tag;
        if(selectedEntity.TryGetComponent<TagComponent>(Tag))
        {
            name = Tag.Tag.c_str();
        }
        ImGui::Text("%s\n", name);

        for(auto&& curr : _EditorLayer->GetActiveScene()->_registry.storage())
        {
            if(auto& storage = curr.second; storage.contains(selectedEntity))
            {
                entt::id_type id = curr.first;
                ImGui::Text(_EditorLayer->GetComponentNameFromEnttId(id).c_str());
            }
        }
        if(ImGui::Button("Add Component")){
            selectedEntity.AddComponent<RenderComponent>();
        }
    }
    else
    {
        ImGui::Text("Nothing selected");

    }
    ImGui::End();

}

    //ANE_LOG_INFO("Entering panel render phase");



