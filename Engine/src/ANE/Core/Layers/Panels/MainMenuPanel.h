#pragma once
#include "UILayerPanel.h"

namespace Engine
{
    class EditorLayer;

    class MainMenuPanel : public UILayerPanel
    {
    public:
        MainMenuPanel(EditorLayer* editorLayer);

        UIUpdateWrapper OnPanelRender() override;
    private:
        ~MainMenuPanel() = default;
    private:
        EditorLayer* _editorLayer;
    };
}
