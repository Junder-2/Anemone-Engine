#include "anepch.h"
#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "InspectorPanel.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Core/Layers/EditorLayer.h"
#include "ANE/Core/Scene/Scene.h"
#include "ANE/Core/Scene/Components/TagComponent.h"
#include "ANE/Core/Scene/Components/UUIDComponent.h"

namespace Engine
{
    SceneHierarchyPanel::SceneHierarchyPanel(EditorLayer* managingLayer)
    {
        _editorLayer = managingLayer;
    }

    void SceneHierarchyPanel::OnPanelRender()
    {
        _activeScene = _editorLayer->GetActiveScene();
        if (!_activeScene) return;

        bool open = true;

        const ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

        ImGui::Begin("Scene Hierarchy", &open, dockSpaceFlags);

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

    void SceneHierarchyPanel::DrawEntityNode(const UUIDComponent& uuid, const TagComponent& tag, const ImGuiTreeNodeFlags nodeFlags) const
    {
        const bool nodeOpen = ImGui::TreeNodeEx(uuid.UUID.c_str(), nodeFlags, tag.Value.c_str());

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            SelectionManager::DeSelect(_selectionContext);
            SelectionManager::RegisterSelect(_selectionContext, uuid.UUID);
        }
        if (nodeOpen)
        {
            //draw children, recursively
            ImGui::TreePop();
        }
    }
}
