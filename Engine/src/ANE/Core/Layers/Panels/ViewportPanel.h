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
        bool _initialized = false;
        EditorLayer* _editorLayer;
    };
}
