#include "anepch.h"
#include "InspectorPanel.h"
#include <imgui.h>
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Core/Layers/EditorLayer.h"
#include "ANE/Math/Random.h"
#include "UIUpdateWrapper.h"
#include "ANE/Core/Scene/Components/Components.h"
#include "ANE/Utilities/MetaUtilities.h"

namespace Engine
{
    template <class T>
    void OnValidate(T* obj)
    {
        if constexpr (requires { obj->OnValidate(); })
            return obj->OnValidate();
        else
        {
        }
    }

    InspectorPanel::InspectorPanel(EditorLayer* editorLayer)
    {
        ANE_PROFILE_FUNCTION();

        _editorLayer = editorLayer;
        for(auto &&[id, type]: entt::resolve()) {
            const std::string componentType{type.info().name()};
            std::string fullString = TypePrefixRemoval(componentType);
            typeMap[fullString] = id;
            componentKeys.push_back(new std::string(fullString));

        }
        for(auto str: componentKeys)
        {
            ANE_ELOG(str->c_str());
        }


    }

    void InspectorPanel::RegisterSelect(const UUIDComponent& selectedEntityID)
    {
        _selected = selectedEntityID.UUID;
    }

    UIUpdateWrapper InspectorPanel::OnPanelRender()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        bool open = true;
        UIUpdateWrapper uiUpdate;
        const ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::Begin("Inspection", &open, dockSpaceFlags);

        const std::vector<std::string>* selectedEntityUUIDS = SelectionManager::GetSelection(SelectionManager::UI);

        if (selectedEntityUUIDS->empty())
        {
            ImGui::Text("Nothing selected");
        }
        else
        {
            Entity selectedEntity = _editorLayer->GetActiveScene()->GetEntityWithUUID(selectedEntityUUIDS->at(0));
            DrawEntityComponentList(selectedEntity);
            if (ImGui::Button("Add Component")) // For physics testing
            {
                ImGui::OpenPopup("my_select_popup");
            }
            {
                static int selected = -1;

                // Simple selection popup (if you want to show the current selection inside the Button itself,
                // you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
                ImGui::SameLine();
                if (ImGui::BeginPopup("my_select_popup"))
                {
                    ImGui::SeparatorText("Components");
                    for (int i = 0; i < componentKeys.size(); i++)
                        if (ImGui::Selectable(componentKeys[i]->c_str()))
                            selected = i;
                    ImGui::EndPopup();
                }
                if(selected != -1)
                {
                    auto str = *componentKeys[selected];
                    ANE_ELOG(str);
                    switch(Hash(str.c_str()))
                    {
                        case Hash("TagComponent"):
                            selectedEntity.AddComponent<TagComponent>();
                            break;
                        case Hash("TransformComponent"):
                            selectedEntity.AddComponent<TransformComponent>();
                            break;
                        case Hash("RenderComponent"):
                            selectedEntity.AddComponent<RenderComponent>("Suzanne.fbx");
                            break;
                        case Hash("ColliderComponent"):
                            selectedEntity.AddComponent<ColliderComponent>(selectedEntity);
                            break;
                        case Hash("CameraComponent"):
                            selectedEntity.AddComponent<CameraComponent>();
                            break;
                        case Hash("RigidBodyComponent"):
                            selectedEntity.AddComponent<RigidBodyComponent>(selectedEntity, BodyType::Static);
                            break;
                        case Hash("NativeScriptComponent"):
                            //selectedEntity.AddComponent<NativeScriptComponent>().Bind<Entity>();
                            break;
                        default:
                            ANE_ELOG("No matching branch statement found that maps this components UI name to a suitable add component call. Try updating the switch statement");
                            break;
                    }
                    selected = -1;

                }
            }
        }


        ImGui::End();

        if (!open) uiUpdate.RemoveSelf = this;
        return uiUpdate;
    }

    std::string InspectorPanel::TypePrefixRemoval(const std::string& fullComponentName)
    {
        const std::string prefix = "struct Engine::";

        // prefix removal
        std::string result1 = fullComponentName.substr(prefix.length());
        return result1;
    }
    void InspectorPanel::DrawEntityComponentList(Entity& selectedEntity)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        for (auto&& [fst, snd] : _editorLayer->GetActiveScene()->_registry.storage())
        {
            if (!snd.contains(selectedEntity)) continue;
            entt::id_type componentTypeID = fst;
            auto type = entt::resolve(componentTypeID);
            if (type)
            {
                auto componentData = type.from_void(snd.value(selectedEntity));
                entt::meta_handle handle = entt::meta_handle(&componentData);
                const std::string componentType{type.info().name()};
                std::string fullString = TypePrefixRemoval(componentType);
                ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanAllColumns;
                if(ImGui::CollapsingHeader(fullString.c_str(),nodeFlags))
                {
                    ImGui::Indent();
                    for (auto&& data : type.data())
                    {
                        auto& field = data.second;
                        bool editable = false;
                        if(field.prop(EDITABLE_HASH))
                        {
                            editable = field.prop(EDITABLE_HASH).value().cast<bool>();
                        }
                        if (editable)
                        {
                            auto itr = g_mutable_data_inspectors.find(field.type().info().hash());
                            if (itr != g_mutable_data_inspectors.end())
                            {
                                if (itr->second(field, componentData))
                                {
                                    //ANE_ELOG("writing to property");
                                    if (auto func = type.func("OnValidate"_hs); func)
                                    {
                                        //ANE_ELOG("Calling on validate");
                                        func.invoke(componentData);
                                    }
                                }
                                else
                                {
                                    //the property was not written too;
                                }
                            }
                            else
                            {
                                std::string string;
                                string.append("No draw function found for mutable data of type: ");
                                string.append(field.type().info().name());
                                ImGui::Text("%s", string.c_str());
                            }
                        }
                        else
                        {
                            auto itr = _immutableDataInspectors.find(field.type().info().hash());
                            if (itr != _immutableDataInspectors.end())
                            {
                                if (itr->second(field, componentData))
                                {
                                }
                                else
                                {
                                    std::string string;
                                    string.append("No draw function found for immutable data of type: ");
                                    string.append(field.type().info().name());
                                    ImGui::Text("%s", string.c_str());
                                }
                            }
                        }
                    }
                    ImGui::Unindent();
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
