#include "anepch.h"
#include "InspectorPanel.h"

#include "imgui.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Math/Random.h"
#include "ANE/Core/Scene/Components/Components.h"

namespace Engine
{
    InspectorPanel::InspectorPanel(EditorLayer* editorLayer)
    {
        _editorLayer = editorLayer;
    }

    void InspectorPanel::RegisterSelect(UUIDComponent selectedEntityID)
    {
        selected = selectedEntityID.UUID;
    }

    void InspectorPanel::WipeSelect()
    {
        selected = "";
    }


    void InspectorPanel::OnPanelRender()
    {
        ImGui::Begin("Inspection");
        std::vector<std::string>* selectedEntityUUIDS = SelectionManager::GetSelection(SelectionManager::UI);

        if (selectedEntityUUIDS->empty())
        {
            ImGui::Text("Nothing selected");
        }
        else
        {
            Entity selectedEntity = _editorLayer->GetActiveScene()->GetEntityWithUUID(selectedEntityUUIDS->at(0));
            DrawEntityComponentList(selectedEntity);
        }
        ImGui::End();
    }

    void InspectorPanel::DrawEntityComponentList(Entity& selectedEntity)
    {
        //Name below!!!
        auto& tag = selectedEntity.GetComponent<TagComponent>().Value; // make sure we always have a tag, this can be dangerous
        char buffer[256] = {};
        const auto error = strcpy_s(buffer, sizeof(buffer), tag.c_str());
        ANE_ASSERT(error == 0, "can't copy string into buffer");



        //GET type list of all types possible
        //Call a Get on each type with the selected entity

        if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
        {
            selectedEntity.GetComponent<TagComponent>().Value = std::string(buffer);

        }
        const auto components = selectedEntity.GetComponent<AttachmentsComponent>().GetComponentList();
        for (Component comp : components)
        {
            //comp.Draw();
        }
        /*

        //Name above!!!
        for (auto&& [fst, snd] : _editorLayer->GetActiveScene()->_registry.storage())
        {
            if (auto& storage = snd; storage.contains(selectedEntity))
            {
                const entt::id_type id = fst;
                ImGui::Text("%s", _editorLayer->GetComponentNameFromEnttId(id).c_str());
            }
        }
        */
    }

    /*
    if(ImGui::Button("Add Physics Suzanne")) // For physics testing
        {
        selectedEntity.GetComponent<TransformComponent>().Transform.AddPosition(Random::InSphere(.2f));
        selectedEntity.AddComponent<RenderComponent>("Suzanne.fbx");
        selectedEntity.AddComponent<RigidBodyComponent>(selectedEntity);
        selectedEntity.AddComponent<ColliderComponent>(selectedEntity, 1.f);
        }
    */
}

//ANE_LOG_INFO("Entering panel render phase");
