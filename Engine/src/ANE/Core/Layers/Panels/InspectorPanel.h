#pragma once
#include <entt.hpp>
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

        static void RegisterSelect(const UUIDComponent& selectedEntityID);
        //static void WipeSelect();
        static std::string TypePrefixRemoval(const std::string& fullComponentName);
        void DrawEntityComponentList(Entity& selectedEntity);

        UIUpdateWrapper OnPanelRender() override;

    private:
        SelectionManager::SelectionContext _selectionContext = SelectionManager::SelectionContext::UI;
        inline static std::string _selected = "";
        EditorLayer* _editorLayer;
        int _style = 0;
    };
}
