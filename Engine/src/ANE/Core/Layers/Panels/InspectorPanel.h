#pragma once
#include <entt.hpp>

#include "UILayerPanel.h"
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

        void OnPanelRender() override;

        void DrawEntityComponentList(Entity& selectedEntity);
        static void RegisterSelect(UUIDComponent selectedEntityID);

    private:
        SelectionManager::SelectionContext _selectionContext = SelectionManager::SelectionContext::UI;
        inline static std::string _selected = "";
        EditorLayer* _editorLayer;
    };
}
