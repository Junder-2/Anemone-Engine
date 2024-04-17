#pragma once
#include "UILayerPanel.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Core/Scene/Scene.h"

namespace Engine
{
    class SceneHierarchyPanel : public UILayerPanel
    {
    public:
        SceneHierarchyPanel(std::unordered_map<const char*, std::shared_ptr<Engine::Scene>>& _currentScenes);
        ~SceneHierarchyPanel();
        void OnPanelRender() override;






        std::unordered_map<const char*, std::shared_ptr<Scene>>* _managedScenes;

        Scene* _sceneContext;
        SelectionManager::selectionContext _SelectionContext = SelectionManager::selectionContext::UI;
    };
}


