#pragma once

#include <entt.hpp>

#include "imgui.h"
#include "UIUpdateWrapper.h"
#include "UILayerPanel.h"

namespace Engine
{
    class EditorLayer;

    class EditorLogPanel : public UILayerPanel
    {

    public:
        EditorLogPanel(EditorLayer* layer);
        ~EditorLogPanel() = default;

        UIUpdateWrapper OnPanelRender() override;

        void ShowLogLevelsPopup();
        void ShowLoggerNamePopup();
        void DrawToolBar();

        void ShowLogFormatPopup();
        void DrawLogMessage(const LogMessage& logMessage);

        static void Clear();

        void LoadSettings();
        void SaveSettings();

        EditorLayer* _editorLayer;

    private:
        static const ImVec4 colorInfo;
        static const ImVec4 colorWarn;
        static const ImVec4 colorError;

        int _levelFilter;
        entt::dense_map<std::string, bool> _loggerNameFilter;

        bool _wrap = true;
        bool _autoScroll = false;

        bool _displayTime = true;
        bool _displaySource = true;
        bool _displayLevel = true;
        bool _displayLoggerName = true;

    };
}
