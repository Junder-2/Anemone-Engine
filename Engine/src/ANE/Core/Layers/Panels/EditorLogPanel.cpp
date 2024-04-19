#include "anepch.h"
#include "EditorLogPanel.h"

#include "imgui.h"

namespace Engine
{
    const ImVec4 EditorLogPanel::COLOR_INFO {0.3f, 0.8f, 0.3f, 1.f };
    const ImVec4 EditorLogPanel::COLOR_WARN {0.8f, 0.8f, 0.f, 1.f};
    const ImVec4 EditorLogPanel::COLOR_ERROR {8.0f, 0.1f, 0.1f, 1.f};

    EditorLogPanel::EditorLogPanel()
    {
    }

    EditorLogPanel::~EditorLogPanel()
    {

    }

    void EditorLogPanel::OnPanelRender()
    {
        auto logMessages = Logging::GetMessages();
        ImGui::Begin("Log Window");

        for (auto logMessage : logMessages)
        {
            if(logMessage.Level == 0) break;

            ImVec4 currentColor;

            switch (logMessage.Level)
            {
                case spdlog::level::trace:
                case spdlog::level::debug:
                case spdlog::level::info:
                currentColor = COLOR_INFO;
                break;
                case spdlog::level::warn:
                currentColor = COLOR_WARN;
                break;
                case spdlog::level::err:
                case spdlog::level::critical:
                currentColor = COLOR_ERROR;
                break;
            }

            ImGui::TextColored(currentColor, "[%s ", logMessage.LoggerName.c_str());
            ImGui::SameLine(0, 0);
            ImGui::TextColored(currentColor, "%s]", logMessage.Time.c_str());
            ImGui::SameLine(0, 0);
            ImGui::TextColored(currentColor, " [%s]", logMessage.Source.c_str());
            ImGui::SameLine(0, 0);
            ImGui::TextColored(currentColor, " [%s]", logMessage.Message.c_str());
        }

        ImGui::End();
    }
}
