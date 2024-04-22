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

        void ShowLogLevelsPopup();
        void DrawToolBar();

        void ShowLogFormatPopup();
        void DrawLogMessage(const LogMessage& logMessage);

        static void Clear();

        void LoadSettings();
        void SaveSettings();

    private:
        static const ImVec4 colorInfo;
        static const ImVec4 colorWarn;
        static const ImVec4 colorError;

        int _levelFilter;

        bool _wrap = true;
        bool _autoScroll = false;

        bool _displayTime = true;
        bool _displaySource = true;
        bool _displayLevel = true;
        bool _displayLoggerName = true;
    };
}
