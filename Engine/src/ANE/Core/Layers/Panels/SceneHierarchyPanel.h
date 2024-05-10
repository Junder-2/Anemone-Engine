#pragma once
#include "UILayerPanel.h"
#include "ANE/Core/Editor/SelectionManager.h"

namespace Engine
{
    typedef int ImGuiTreeNodeFlags;

    class Scene;
    class EditorLayer;
    struct TagComponent;
    struct UUIDComponent;

    class SceneHierarchyPanel : public UILayerPanel
    {
    public:
        SceneHierarchyPanel(EditorLayer* managingLayer);
        ~SceneHierarchyPanel() = default;
        void DrawEntityNode(const UUIDComponent& uuid, const TagComponent& tag, ImGuiTreeNodeFlags nodeFlags) const;
        void DrawEntityNodeList() const;
        UIUpdateWrapper OnPanelRender() override;

    private:
        //std::unordered_map<const char*, std::shared_ptr<Scene>>* _managedScenes;
        EditorLayer* _editorLayer;
        std::shared_ptr<Scene> _activeScene;
        SelectionManager::SelectionContext _selectionContext = SelectionManager::SelectionContext::UI;
    };
}


