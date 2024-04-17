#include "anepch.h"
#include "SceneHierarchyPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "InspectorPanel.h"
#include "ANE/Core/Scene/Components/TagComponent.h"
#include "ANE/Core/Scene/Components/UUIDComponent.h"


Engine::SceneHierarchyPanel::SceneHierarchyPanel(std::unordered_map<const char*, std::shared_ptr<Engine::Scene>>& currentScenes)
{
    _managedScenes = &currentScenes;

}

void Engine::SceneHierarchyPanel::OnPanelRender()
{
    ImGui::Begin("Scene Hierarchy");
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
     //ANE_LOG_INFO("Entering panel render phase");

        for (auto& it: *_managedScenes) {
            if (ImGui::CollapsingHeader(it.first))
            {
                ImGui::SeparatorText("General");

                auto IDView = it.second->_registry.view<UUIDComponent,TagComponent>();
                for(auto entity: IDView)
                {
                    auto &UUID = IDView.get<UUIDComponent>(entity);
                    auto &Tag = IDView.get<TagComponent>(entity);
                    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

                    bool node_open = ImGui::TreeNodeEx(Tag.Tag.c_str(), node_flags, Tag.Tag.c_str());

                    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                    {
                        InspectorPanel::RegisterSelect(UUID);
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
