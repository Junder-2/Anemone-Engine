#pragma once
#include <entt.hpp>

#include "UILayerPanel.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Core/Layers/EditorLayer.h"

namespace Engine
{
    class InspectorPanel : public UILayerPanel
    {
    public:

        InspectorPanel();
        InspectorPanel(EditorLayer* editorLayer);
        ~InspectorPanel();


static void RegisterSelect(UUIDComponent selectedEntityID);
static void WipeSelect();
        SelectionManager::selectionContext _SelectionContext = SelectionManager::selectionContext::UI;

        inline static std::string selected = "";
        EditorLayer* _EditorLayer;
        void OnPanelRender() override;
    };
}
