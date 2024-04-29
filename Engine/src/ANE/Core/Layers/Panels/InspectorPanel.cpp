#include "anepch.h"
#include "InspectorPanel.h"
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <cstring>
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
        bool open = true;

        const ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::Begin("Inspection", &open, dockSpaceFlags);

        std::vector<std::string>* selectedEntityUUIDS = SelectionManager::GetSelection(SelectionManager::UI);

        if (selectedEntityUUIDS->empty())
        {
            ImGui::Text("Nothing selected");
        }
        else
        {
            Entity selectedEntity = _editorLayer->GetActiveScene()->GetEntityWithUUID(selectedEntityUUIDS->at(0));
            DrawEntityComponentList(selectedEntity);
            if (ImGui::Button("Add Physics Suzanne")) // For physics testing
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
                    entt::id_type componentTypeID = fst;
                    auto type = entt::resolve(componentTypeID);
                    if (type)
                    {
                        auto componentData = type.from_void(snd.value(selectedEntity));

                        const std::string componentType{type.info().name()};
                        std::string fullString = TypePrefixRemoval(componentType);
                        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
                        bool open = ImGui::CollapsingHeader(fullString.c_str(),node_flags);
                        if(open)
                        {
                            for (auto&& data : type.data())
                            {
                                auto& field = data.second;
                                bool editable = false;
                                if(field.prop(EDITABLEHASH))
                                {
                                    editable = field.prop(EDITABLEHASH).value().cast<bool>();

                                }
                                if(editable)
                                {
                                    auto itr = g_mutable_data_inspectors.find(field.type().info().hash());
                                    if (itr != g_mutable_data_inspectors.end())
                                    {
                                        if(itr->second(field, componentData))

                                        {
                                            //selectedEntity.OnValidate();
                                            ANE_ELOG("OnValidate should occur here");
                                        }
                                        else
                                        {
                                            std::string string;
                                            string.append("No draw function found for data of type: ");
                                            string.append(field.type().info().name());
                                            ImGui::Text("%s", string.c_str());
                                        }
                                    }

                                }
                                else
                                {
                                    auto itr = g_immutable_data_inspectors.find(field.type().info().hash());
                                    if (itr != g_immutable_data_inspectors.end())
                                    {
                                        if(itr->second(field, componentData))

                                        {
                                            //selectedEntity.OnValidate();
                                        }
                                        else
                                        {
                                            std::string string;
                                            string.append("No draw function found for data of type: ");
                                            string.append(field.type().info().name());
                                            ImGui::Text("%s", string.c_str());
                                        }
                                    }
                                }

                            }
                        }
                    }
                    else
                    {
                        std::string string;
                        string.append("Component Type could not be resolved: ");
                        string.append(std::to_string(componentTypeID));
                        ImGui::Text("%s", string.c_str());
                    }
                }
            }
        }
    }
}
