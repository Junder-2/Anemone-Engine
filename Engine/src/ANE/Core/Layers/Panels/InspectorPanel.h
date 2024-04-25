#pragma once
#include <entt.hpp>
#include "imgui.h"
#include "ANE/Utilities/MetaUtilities.h"
#include "UILayerPanel.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Core/Layers/EditorLayer.h"

namespace Engine
{

    class InspectorPanel : public UILayerPanel
    {
    public:
        InspectorPanel(EditorLayer* editorLayer);
        ~InspectorPanel() = default;


        std::string TypePrefixRemoval(std::string fullComponentName);

        static void RegisterSelect(UUIDComponent selectedEntityID);
        static void WipeSelect();
        SelectionManager::selectionContext _SelectionContext = SelectionManager::selectionContext::UI;
        void DrawEntityComponentList(Entity& selectedEntity);
        inline static std::string selected = "";
        EditorLayer* _editorLayer;
        void OnPanelRender() override;


    };
}
