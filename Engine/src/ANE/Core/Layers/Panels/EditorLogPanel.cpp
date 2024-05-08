#include "anepch.h"
#include "EditorLogPanel.h"

#include <ranges>
#include "imgui.h"
#include "imgui_internal.h"
#include "ANE/Utilities/LoggingUtilities.h"
#include "ANE/Core/Layers/EditorLayer.h"
#include "ANE/Input/EditorInputSystem.h"
#include "ANE/Input/Input.h"
#include "ANE/Math/FMath.h"
#include "ANE/Math/Types/Vector2.h"
#include "ANE/Utilities/ColorUtilities.h"

namespace Engine
{
    //TODO: I just choose some random colors
    const ImVec4 EditorLogPanel::colorInfo {0.3f, 0.8f, 0.3f, 1.f };
    const ImVec4 EditorLogPanel::colorWarn {0.8f, 0.8f, 0.f, 1.f};
    const ImVec4 EditorLogPanel::colorError {1.0f, 0.1f, 0.1f, 1.f};

    EditorLogPanel::EditorLogPanel(EditorLayer* layer)
    {
        _levelFilter = EnumCast(LogLevelCategory::Error | LogLevelCategory::Warn | LogLevelCategory::Info | LogLevelCategory::Debug | LogLevelCategory::Trace);

        for (const auto& loggerName : Logging::GetRegisteredLoggerNames())
        {
            if(!_loggerNameFilter.contains(loggerName))
            {
                _loggerNameFilter.insert_or_assign(loggerName, true);
            }
        }
        _editorLayer = layer;
    }

    UIUpdateWrapper EditorLogPanel::OnPanelRender()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        UIUpdateWrapper UIUpdate;

        bool open;

        if(ImGui::Begin("Log Window", &open))
        {
            DrawToolBar();

            ImGui::Separator();

            const ImGuiWindowFlags flags = (_wrap ? 0 : ImGuiWindowFlags_HorizontalScrollbar) | (_autoScroll ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs : 0);
            ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, flags);

            const auto logMessages = Logging::GetMessages();

            int messageIndex = 0;
            // Using a reverse iterator because imgui draws the text top to bottom and the latest message is first
            for (const LogMessage& logMessage : logMessages | std::views::reverse)
            {
                if(logMessage.LevelCategory == LogLevelCategory::None) continue;
                if(!_loggerNameFilter[logMessage.LoggerName]) continue;
                if(!(EnumCast(logMessage.LevelCategory) & _levelFilter)) continue;

                DrawLogMessage(logMessage, messageIndex);
                messageIndex++;
            }

            if (_autoScroll)
            {
                ImGui::SetScrollHereY(1.f);
            }
            ImGui::EndChild();
        }

        ImGui::End();
        if (!open) UIUpdate.RemoveSelf = this;
        return UIUpdate;
    }

    void EditorLogPanel::ShowLogLevelsPopup()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        ImGui::MenuItem("Filter Levels", nullptr, false, false);
        bool filterTrace = _levelFilter & EnumCast(LogLevelCategory::Trace);
        if(ImGui::Checkbox("Trace", &filterTrace))
        {
            _levelFilter ^= EnumCast(LogLevelCategory::Trace);
        }
        bool filterDebug = _levelFilter & EnumCast(LogLevelCategory::Debug);
        if(ImGui::Checkbox("Debug", &filterDebug))
        {
            _levelFilter ^= EnumCast(LogLevelCategory::Debug);
        }
        bool filterInfo = _levelFilter & EnumCast(LogLevelCategory::Info);
        if(ImGui::Checkbox("Info", &filterInfo))
        {
            _levelFilter ^= EnumCast(LogLevelCategory::Info);
        }
        bool filterWarn = _levelFilter & EnumCast(LogLevelCategory::Warn);
        if(ImGui::Checkbox("Warn", &filterWarn))
        {
            _levelFilter ^= EnumCast(LogLevelCategory::Warn);
        }
        bool filterError = _levelFilter & EnumCast(LogLevelCategory::Error);
        if(ImGui::Checkbox("Error", &filterError))
        {
            _levelFilter ^= EnumCast(LogLevelCategory::Error);
        }
    }

    void EditorLogPanel::ShowLoggerNamePopup()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        ImGui::MenuItem("Loggers filter", nullptr, false, false);

        for (const auto& loggerName : Logging::GetRegisteredLoggerNames())
        {
            if(!_loggerNameFilter.contains(loggerName))
            {
                _loggerNameFilter.insert_or_assign(loggerName, true);
            }
            bool* isActive = &_loggerNameFilter[loggerName];
            ImGui::Checkbox(loggerName.c_str(), isActive);
        }
    }

    void EditorLogPanel::ShowLogFormatPopup()
    {
        ImGui::MenuItem("Logging Format", nullptr, false, false);
        ImGui::Checkbox("Time", &_displayTime);
        ImGui::Checkbox("Level", &_displayLevel);
        ImGui::Checkbox("Source", &_displaySource);
        ImGui::Checkbox("Logger", &_displayLoggerName);
    }

    void EditorLogPanel::DrawLogMessage(const LogMessage& logMessage)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        ImGui::BeginGroup();

        if(_wrap)
        {
            ImGui::PushTextWrapPos(0);
        }

        ImVec4 currentColor = colorError;

        switch (logMessage.LevelCategory)
        {
            case LogLevelCategory::Trace:
            case LogLevelCategory::Debug:
            case LogLevelCategory::Info:
                currentColor = colorInfo;
                break;
            case LogLevelCategory::Warn:
                currentColor = colorWarn;
                break;
            case LogLevelCategory::Error:
                currentColor = colorError;
                break;
        }

        const std::string levelName = LoggingUtilities::ToString(logMessage.LevelCategory);

        // Right now using append, from searches ostream is slower?
        std::string fullMessage;
        if(_displayLoggerName && _displayLevel)
        {
            fullMessage.append(std::format("[{0} {1}", logMessage.LoggerName, levelName));
        }
        else if(_displayLoggerName)
        {
            fullMessage.append(std::format("[{0}", logMessage.LoggerName));
        }
        else if(_displayLevel)
        {
            fullMessage.append(std::format("[{0}", levelName));
        }

        if(_displayTime && (_displayLevel || _displayLoggerName))
        {
            fullMessage.append(std::format(" {0}] ", logMessage.Time));
        }
        else if(_displayTime)
        {
            fullMessage.append(std::format("[{0}] ", logMessage.Time));
        }
        else if((_displayLevel || _displayLoggerName))
        {
            fullMessage.append("] ");
        }

        if(_displaySource)
        {
            fullMessage.append(std::format("[{0}] ", logMessage.Source));
        }

        fullMessage.append(logMessage.Message);

        ImGui::TextColored(currentColor, "%s", fullMessage.c_str());

        if(_wrap)
        {
            ImGui::PopTextWrapPos();
        }

        ImGui::EndGroup();

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("%s", logMessage.Source.c_str());
        }
    }

    void EditorLogPanel::DrawToolBar()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if (ImGui::Button("Loggers"))
        {
            ImGui::OpenPopup("LoggerNamesPopup");
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Filter out certain loggers");

        if (ImGui::BeginPopup("LoggerNamesPopup"))
        {
            ShowLoggerNamePopup();
            ImGui::EndPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Levels"))
        {
            ImGui::OpenPopup("LogLevelsPopup");
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Change the logging levels");

        if (ImGui::BeginPopup("LogLevelsPopup"))
        {
            ShowLogLevelsPopup();
            ImGui::EndPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Display"))
        {
            ImGui::OpenPopup("LogFormatPopup");
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Log message display options");

        if (ImGui::BeginPopup("LogFormatPopup"))
        {
            ShowLogFormatPopup();
            ImGui::EndPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Clear"))
        {
            Clear();
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Clears log message list");

        ImGui::SameLine();
        ImGui::Checkbox("Wrap", &_wrap);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Wraps log messages");

        ImGui::SameLine();
        ImGui::Checkbox("Auto Scroll", &_autoScroll);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Auto scrolling");
    }

    void EditorLogPanel::Clear()
    {
        Logging::ClearMessages();
    }

    void EditorLogPanel::LoadSettings()
    {
        // If we want to save log display options add here
    }

    void EditorLogPanel::SaveSettings()
    {

    }
}
