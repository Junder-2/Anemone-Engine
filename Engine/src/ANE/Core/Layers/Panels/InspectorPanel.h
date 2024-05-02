#pragma once
#include <entt.hpp>
#include "imgui.h"
#include "ANE/Utilities/MetaUtilities.h"
#include "UILayerPanel.h"
#include "UIUpdateWrapper.h"
#include "ANE/Core/Editor/SelectionManager.h"

namespace Engine
{

    struct UUIDComponent;
    class Entity;
    class EditorLayer;

    class InspectorPanel : public UILayerPanel
    {
    public:
        InspectorPanel(EditorLayer* editorLayer);
        ~InspectorPanel() = default;


        std::string TypePrefixRemoval(std::string fullComponentName);

        static void RegisterSelect(UUIDComponent selectedEntityID);
        //static void WipeSelect();
        void DrawEntityComponentList(Entity& selectedEntity);

        UIUpdateWrapper OnPanelRender() override;

    private:
        SelectionManager::SelectionContext _selectionContext = SelectionManager::SelectionContext::UI;
        inline static std::string _selected = "";
        EditorLayer* _editorLayer;
        int style = 0;

    };
}
