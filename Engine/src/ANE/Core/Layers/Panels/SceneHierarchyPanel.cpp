#include "anepch.h"
#include "SceneHierarchyPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "InspectorPanel.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Core/Scene/Components/TagComponent.h"
#include "ANE/Core/Scene/Components/UUIDComponent.h"


Engine::SceneHierarchyPanel::SceneHierarchyPanel(std::unordered_map<const char*, std::shared_ptr<Engine::Scene>>& currentScenes)
{
    _managedScenes = &currentScenes;

}

void Engine::SceneHierarchyPanel::OnPanelRender()
{
    ImGui::Begin("Scene Hierarchy");
    std::vector<std::string>* selectedEntityUUIDS = SelectionManager::GetSelection(SelectionManager::UI);

     //ANE_LOG_INFO("Entering panel render phase");

        for (auto& it: *_managedScenes) {
            if (ImGui::CollapsingHeader(it.first))
            {
                auto IDView = it.second->_registry.view<UUIDComponent,TagComponent>();
                for(auto entity: IDView)
                {
                    auto &UUID = IDView.get<UUIDComponent>(entity);
                    auto &Tag = IDView.get<TagComponent>(entity);
                    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

                    if(std::find(selectedEntityUUIDS->begin(), selectedEntityUUIDS->end(), UUID.UUID) != selectedEntityUUIDS->end()){
                        node_flags |= ImGuiTreeNodeFlags_Selected;
                    }

                    bool node_open = ImGui::TreeNodeEx(Tag.Tag.c_str(), node_flags, Tag.Tag.c_str());

                    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                    {
                        SelectionManager::DeSelect(_SelectionContext);
                        SelectionManager::RegisterSelect(_SelectionContext,UUID.UUID);
                    }
                    if(node_open)
                    {
                        ImGui::TreePop();
                    }



                }
            }

        }
    ImGui::End();

}
