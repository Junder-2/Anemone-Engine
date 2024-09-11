#pragma once

#include "UILayerPanel.h"

#include <entt.hpp>
#include "ANE/Core/Log/LoggingTypes.h"
#include "ANE/Math/Types/Vector4.h"

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
        void DrawLogMessage(const LogMessage& logMessage, int index) const;

        static void Clear();

        void LoadSettings();
        void SaveSettings();

    private:
        void OnScroll(Vector2 delta);
        void ResizeLoggerFilter();

    private:
        static const Vector4 colorTrace;
        static const Vector4 colorInfo;
        static const Vector4 colorWarn;
        static const Vector4 colorError;

        LogLevelCategories _levelFilter;
        std::vector<bool> _loggerIdFilter;

        bool _loggerNameFilterOpen;

        bool _wrap = true;
        bool _autoScroll = true;

        bool _displayTime = true;
        bool _displaySource = false;
        bool _displayLevel = true;
        bool _displayLoggerName = true;

        Vector4 _panelRect;

        EditorLayer* _editorLayer;
    };
}
