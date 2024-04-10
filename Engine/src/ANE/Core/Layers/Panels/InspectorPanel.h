#pragma once
#include "UILayerPanel.h"
#include "ANE/Core/Scene/Scene.h"

namespace Engine
{
    class InspectorPanel : public UILayerPanel
    {
    public:
        InspectorPanel(std::unordered_map<const char*, std::shared_ptr<Engine::Scene>>& _currentScenes);
        ~InspectorPanel();
        void OnPanelRender() override;


        std::unordered_map<const char*, std::shared_ptr<Scene>>* _managedScenes;

        Scene* _sceneContext;
    };
}
