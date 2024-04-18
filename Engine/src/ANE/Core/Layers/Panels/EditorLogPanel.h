#pragma once

#include "imgui.h"
#include "UILayerPanel.h"

namespace Engine
{
    class EditorLogPanel : public UILayerPanel
    {
    public:
        EditorLogPanel();
        ~EditorLogPanel();
        void OnPanelRender() override;

    private:
        static const ImVec4 COLOR_INFO;
        static const ImVec4 COLOR_WARN;
        static const ImVec4 COLOR_ERROR;
    };
}
