#pragma once
#include "imgui.h"
#include "UILayerPanel.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Core/Layers/EditorLayer.h"
#include "ANE/Core/Scene/Scene.h"
#include "ANE/Core/Scene/Components/TagComponent.h"
#include "ANE/Core/Scene/Components/UUIDComponent.h"

namespace Engine
{
    class SceneHierarchyPanel : public UILayerPanel
    {
    public:
        ~SceneHierarchyPanel() = default;
        void DrawEntityNode(const UUIDComponent& UUID, const TagComponent& Tag, ImGuiTreeNodeFlags node_flags) const;
        void DrawEntityNodeList() const;
        SceneHierarchyPanel(EditorLayer* ManagingLayer);
        void OnPanelRender() override;


        //std::unordered_map<const char*, std::shared_ptr<Scene>>* _managedScenes;
        EditorLayer* _editorLayer;
        std::shared_ptr<Scene> _activeScene;
        SelectionManager::selectionContext _SelectionContext = SelectionManager::selectionContext::UI;
    };
}


