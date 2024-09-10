#include "anepch.h"
#include "EditorLogPanel.h"

#include <ranges>
#include "imgui.h"
#include "imgui_internal.h"
#include "ANE/Core/Layers/EditorLayer.h"
#include "ANE/Core/Log/LogMessage.h"
#include "ANE/Input/EditorInputSystem.h"
#include "ANE/Input/Input.h"
#include "ANE/Math/FMath.h"
#include "ANE/Math/Types/Vector2.h"
#include "ANE/Utilities/ColorUtilities.h"

namespace Engine
{
    const Vector4 EditorLogPanel::colorTrace {ColorUtilities::HexToRGB(0x66b2cd), 1.f };
    const Vector4 EditorLogPanel::colorInfo {ColorUtilities::HexToRGB(0x37d274), 1.f };
    const Vector4 EditorLogPanel::colorWarn {ColorUtilities::HexToRGB(0xe7d653), 1.f};
    const Vector4 EditorLogPanel::colorError {ColorUtilities::HexToRGB(0xc03244), 1.f};

    EditorLogPanel::EditorLogPanel(EditorLayer* layer)
    {
        _levelFilter = EnumCast(LogLevelCategory::Error | LogLevelCategory::Warn | LogLevelCategory::Info | LogLevelCategory::Debug | LogLevelCategory::Trace);

        _loggerIdFilter.clear();

        _editorLayer = layer;

        GetEditorInputSystem().BindMouseScroll(MakeDelegate(this, &EditorLogPanel::OnScroll));
    }

    UIUpdateWrapper EditorLogPanel::OnPanelRender()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        UIUpdateWrapper UIUpdate;

        bool open;

        if(ImGui::Begin("Log Window", &open))
        {
            if(_loggerIdFilter.empty()) // Makes sure loggerfilter is populated
            {
                ResizeLoggerFilter();
            }

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
                if(!_loggerIdFilter[logMessage.LoggerNameIndex]) continue;
                if(!(EnumCast(logMessage.LevelCategory) & _levelFilter)) continue;

                DrawLogMessage(logMessage, messageIndex);
                messageIndex++;
            }

            if (_autoScroll)
            {
                ImGui::SetScrollHereY(1.f);
            }
            ImGui::EndChild();

            _panelRect.X = ImGui::GetItemRectMin().x;
            _panelRect.Y = ImGui::GetItemRectMin().y;
            _panelRect.Z = ImGui::GetItemRectMax().x;
            _panelRect.W = ImGui::GetItemRectMax().y;
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

        const auto loggerNames = Logging::GetRegisteredLoggerNames();

        for (int i = 0; i < loggerNames.size(); ++i)
        {
            bool* isActive = reinterpret_cast<bool*>(&_loggerIdFilter[i]); // vector<bool> is encoded is such a way that it cant be referenced
            ImGui::Checkbox(loggerNames[i].c_str(), isActive);
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

    void EditorLogPanel::DrawLogMessage(const LogMessage& logMessage, int index) const
    {
        ANE_DEEP_PROFILE_FUNCTION();

        ImGui::BeginGroup();

        if(_wrap)
        {
            ImGui::PushTextWrapPos(0);
        }

        Vector4 currentColor = colorError;

        switch (logMessage.LevelCategory)
        {
            case LogLevelCategory::Trace:
            case LogLevelCategory::Debug:
                currentColor = colorTrace;
                break;
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

        std::string fullMessage = logMessage.ConstructMessage(_displayLoggerName, _displayLevel, _displayTime, _displaySource);

        ImGui::TextColored(currentColor, "%s", fullMessage.c_str());

        if(_wrap)
        {
            ImGui::PopTextWrapPos();
        }

        ImGui::EndGroup();

        if(index % 2 != 0)
        {
            const auto window = ImGui::GetCurrentWindow();
            const auto style = ImGui::GetStyle();

            auto bgMin = ImGui::GetItemRectMin();
            bgMin.y -= style.CellPadding.y;
            bgMin.x = window->WorkRect.Min.x - style.CellPadding.x;
            auto bgMax = ImGui::GetItemRectMax();
            bgMax.y += style.CellPadding.y;
            bgMax.x = window->WorkRect.Max.x + style.CellPadding.x;

            ImGui::GetWindowDrawList()->AddRectFilled(bgMin, bgMax, IM_COL32(0,0,0,15), style.FrameRounding, ImDrawFlags_RoundCornersDefault_);
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("%s", logMessage.RetrieveSource().c_str());
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
            if (!_loggerNameFilterOpen) // To only update when opening initially
            {
                ResizeLoggerFilter();
                _loggerNameFilterOpen = true;
            }

            ShowLoggerNamePopup();
            ImGui::EndPopup();
        }
        else
        {
            _loggerNameFilterOpen = false;
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

    void EditorLogPanel::OnScroll(const Vector2 delta)
    {
        if(FMath::Abs(delta.Y) < 1) return;
        if(!ImGui::IsMouseHoveringRect(ImVec2(_panelRect.X, _panelRect.Y), ImVec2(_panelRect.Z, _panelRect.W), false)) return;

        _autoScroll = false;
    }

    void EditorLogPanel::ResizeLoggerFilter()
    {
        const auto loggerNames = Logging::GetRegisteredLoggerNames();

        if (loggerNames.size() > _loggerIdFilter.size())
        {
            _loggerIdFilter.resize(loggerNames.size(), true);
        }
    }
}
