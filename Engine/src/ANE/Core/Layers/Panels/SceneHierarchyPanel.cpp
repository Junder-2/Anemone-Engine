#include "anepch.h"
#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "InspectorPanel.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Core/Scene/Components/TagComponent.h"
#include "ANE/Core/Scene/Components/UUIDComponent.h"


namespace Engine
{
    SceneHierarchyPanel::SceneHierarchyPanel(EditorLayer* ManagingLayer)
    {
        _editorLayer = ManagingLayer;
    }


    void SceneHierarchyPanel::OnPanelRender()
    {
        _activeScene = _editorLayer->GetActiveScene();
        if (!_activeScene) return;

        bool open = true;

        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

        ImGui::Begin("Scene Hierarchy", &open, dockspace_flags);

        DrawEntityNodeList();

        ImGui::End();
    }


    void SceneHierarchyPanel::DrawEntityNodeList() const
    {
        std::vector<std::string>* selectedEntityUUIDS = SelectionManager::GetSelection(SelectionManager::UI);
        for (const auto IDView = _activeScene->_registry.view<UUIDComponent, TagComponent>(); const auto entity : IDView)
        {
            auto& UUID = IDView.get<UUIDComponent>(entity);
            auto& Tag = IDView.get<TagComponent>(entity);
            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

            if (std::ranges::find(*selectedEntityUUIDS, UUID.UUID) != selectedEntityUUIDS->end())
            {
                node_flags |= ImGuiTreeNodeFlags_Selected;
            }
            DrawEntityNode(UUID, Tag, node_flags);
        }
    }

    void SceneHierarchyPanel::DrawEntityNode(const UUIDComponent& UUID, const TagComponent& Tag, ImGuiTreeNodeFlags node_flags) const
    {
        const bool nodeOpen = ImGui::TreeNodeEx(UUID.UUID.c_str(), node_flags, Tag.Value.c_str());

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            SelectionManager::DeSelect(_SelectionContext);
            SelectionManager::RegisterSelect(_SelectionContext, UUID.UUID);
        }
        if (nodeOpen)
        {
            //draw children, recursively
            ImGui::TreePop();
        }
    }
}
