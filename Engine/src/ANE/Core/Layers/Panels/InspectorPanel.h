#pragma once
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

        static void RegisterSelect(const UUIDComponent& selectedEntityID);
        //static void WipeSelect();
        static std::string TypePrefixRemoval(const std::string& fullComponentName);

        static constexpr unsigned int Hash(const char *s, int off = 0) {
            return !s[off] ? 5381 : (Hash(s, off+1)*33) ^ s[off];
        }

        void DrawEntityComponentList(Entity& selectedEntity);

        UIUpdateWrapper OnPanelRender() override;

    private:
        SelectionManager::SelectionContext _selectionContext = SelectionManager::SelectionContext::UI;
        inline static std::string _selected = "";

        std::unordered_map<std::string,entt::id_type> typeMap;
        std::vector<entt::id_type> componentValues;
        std::vector<std::string*> componentKeys;
        EditorLayer* _editorLayer;
        int _style = 0;
    };
}
