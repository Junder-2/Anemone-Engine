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
            if(ImGui::Button("Add Physics Suzanne")) // For physics testing
                {
                selectedEntity.GetComponent<TransformComponent>().Transform.AddPosition(Random::InSphere(.2f));
                selectedEntity.AddComponent<RenderComponent>("Suzanne.fbx");
                selectedEntity.AddComponent<RigidBodyComponent>(selectedEntity);
                selectedEntity.AddComponent<ColliderComponent>(selectedEntity, 1.f);
                }
        }

        ImGui::End();
    }
    std::string InspectorPanel::TypePrefixRemoval(std::string fullComponentName)
    {
        std::string prefix = "struct Engine::";

        // prefix removal
        std::string result1 = fullComponentName.substr(prefix.length());
        return result1;
    }

    void InspectorPanel::DrawEntityComponentList(Entity& selectedEntity)
    {
        {

            for (auto&& [fst, snd] : _editorLayer->GetActiveScene()->_registry.storage())
            {
                if (snd.contains(selectedEntity))
                {
                    entt::id_type component_type_id = fst;
                    auto type = entt::resolve(component_type_id);
                    if (type)
                    {
                        auto component_data = type.from_void(snd.value(selectedEntity));
                        std::string component_type{type.info().name()};
                        std::string full_string = TypePrefixRemoval(component_type);
                        ImGui::Text(full_string.c_str(), ImGuiTreeNodeFlags_DefaultOpen);

                        for (auto&& data : type.data())
                        {
                            auto& field = data.second;

                            auto itr = g_data_inspectors.find(field.type().info().hash());
                            if (itr != g_data_inspectors.end())
                            {
                                itr->second(field, component_data);
                            }
                            else
                            {
                                ImGui::Text("%s", std::to_string(field.type().info().hash()));
                            }
                        }


                    }
                }
            }
        }
    }
}

    /*
        //Name below!!!
        auto& tag = selectedEntity.GetComponent<TagComponent>().Value; // make sure we always have a tag, this can be dangerous
        char buffer[256] = {};
        const auto error = strcpy_s(buffer, sizeof(buffer), tag.c_str());
        ANE_ASSERT(error == 0, "can't copy string into buffer");

g_data_inspectors.find()

        //GET type list of all types possible
        //Call a Get on each type with the selected entity

        if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
        {
            selectedEntity.GetComponent<TagComponent>().Value = std::string(buffer);
        }
    }

        for(auto &&[id, type]: entt::resolve()) {
            if(selectedEntity.HasComponent<id>())
            // ...
        }
        for (auto&& curr : _editorLayer->GetActiveScene()->_registry.storage())
        {
            if (curr.second.contains(selectedEntity))
            {
                entt::id_type component_type_id = curr.first;
                auto type = entt::resolve(component_type_id);
                if (type)
                {
                    auto component_data = type.from_void(_editorLayer->GetActiveScene()->_registry.get<component_type_id>(selectedEntity));

                    std::string component_type{ type.info().name() };
                    if (ImGui::CollapsingHeader(component_type.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        for (auto&& data : type.data())
                        {
                            auto& field = data.second;

                            auto itr = g_data_inspectors.find(field.type().info().hash());
                            if (itr != g_data_inspectors.end())
                            {
                                itr->second(field, component_data);
                            }
                            else
                            {
                                ImGui::Text(field.prop("display_name").value().cast<char const*>());
                            }
                        }
                    }
                }
            }
        }
    }
        /*
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




    /*


}
*/
//ANE_LOG_INFO("Entering panel render phase");
