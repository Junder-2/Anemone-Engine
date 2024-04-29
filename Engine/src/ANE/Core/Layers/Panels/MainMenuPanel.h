#pragma once
#include "UILayerPanel.h"
#include "UIUpdateWrapper.h"
#include "ANE/Core/Layers/EditorLayer.h"

namespace Engine
{
    class MainMenuPanel : public UILayerPanel
    {

    private:
        EditorLayer* _editorLayer;
        ~MainMenuPanel() = default;
    public:
        MainMenuPanel(EditorLayer* editorLayer);
        UIUpdateWrapper OnPanelRender() override;
    };
}
