#include "anepch.h"
#include "InspectorPanel.h"

#include "imgui.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Core/Layers/EditorLayer.h"
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
        _selected = selectedEntityID.UUID;
    }

    void InspectorPanel::OnPanelRender()
    {
        ANE_DEEP_PROFILE_FUNCTION();

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
        ImGui::ShowDemoWindow();
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

