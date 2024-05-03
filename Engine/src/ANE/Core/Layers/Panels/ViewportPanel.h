#pragma once
#include "UILayerPanel.h"

namespace Engine
{
    class EditorLayer;

    class ViewportPanel final : public UILayerPanel
    {
    public:
        ViewportPanel(EditorLayer* editorLayer);
        ~ViewportPanel();

        UIUpdateWrapper OnPanelRender() override;

    private:
        static void ViewMenu();
        static void ViewPhysicsDebugMenu();
    private:
        bool _initialized = false;
        EditorLayer* _editorLayer;
    };
}
