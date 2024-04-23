#include "anepch.h"
#include "InspectorPanel.h"

#include "imgui.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Math/Random.h"
#include "ANE/Core/Scene/Components/ColliderComponent.h"
#include "ANE/Core/Scene/Components/RenderComponent.h"
#include "ANE/Core/Scene/Components/RigidBodyComponent.h"


Engine::InspectorPanel::InspectorPanel(EditorLayer* editorLayer)
{
    _editorLayer = editorLayer;
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

    if (!selectedEntityUUIDS->empty())
    {
        Entity selectedEntity = _editorLayer->GetActiveScene()->GetEntityWithUUID(selectedEntityUUIDS->at(0));

        auto& tag = selectedEntity.GetComponent<TagComponent>().Value; // make sure we always have a tag, this can be dangerous


        for (auto&& [fst, snd] : _editorLayer->GetActiveScene()->_registry.storage())
        {
            if (auto& storage = snd; storage.contains(selectedEntity))
            {
                const entt::id_type id = fst;
                ImGui::Text("%s", _editorLayer->GetComponentNameFromEnttId(id).c_str());
            }
        }

        char buffer[256] = {};

        const auto error = strcpy_s(buffer,sizeof(buffer),  tag.c_str());

        ANE_ASSERT(error == 0, "can't copy string into buffer");

        if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
            selectedEntity.GetComponent<TagComponent>().Value = std::string(buffer);

        if(ImGui::Button("Add Component")){
            selectedEntity.GetComponent<TransformComponent>().Transform.AddPosition(Random::InSphere(.2f));
            selectedEntity.AddComponent<RenderComponent>("Suzanne.fbx");
            selectedEntity.AddComponent<RigidBodyComponent>(selectedEntity);
            selectedEntity.AddComponent<ColliderComponent>(selectedEntity, 1.f);
        }
    }
    else
    {
        ImGui::Text("Nothing selected");
    }
    ImGui::End();
}

//ANE_LOG_INFO("Entering panel render phase");
