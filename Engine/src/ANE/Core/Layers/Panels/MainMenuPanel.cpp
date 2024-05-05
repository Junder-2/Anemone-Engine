#include "anepch.h"
#include "MainMenuPanel.h"

#include "CreateScenePanel.h"
#include "EditorLogPanel.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "InspectorPanel.h"
#include "SceneHierarchyPanel.h"

namespace Engine
{
    MainMenuPanel::MainMenuPanel(EditorLayer* editorLayer)
    {
        _editorLayer = editorLayer;
    }

    UIUpdateWrapper MainMenuPanel::OnPanelRender()
    {
        UIUpdateWrapper UIUpdate;

        constexpr auto imBlack = ImVec4(0, 0, 0, 1);
        constexpr auto imClear = ImVec4(0, 0, 0, 0);

        constexpr ImVec4 aneBlack = ImVec4(0.10f, 0.11f, 0.18f, 1.00f);
        const ImVec4 aneFrame = ImLerp(imClear, aneBlack, .5f);
        const ImVec4 aneFrameActive = ImLerp(aneFrame, imBlack, .2f);

        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, aneFrameActive);
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Create Scene")) UIUpdate.AddPanel(new CreateScenePanel(_editorLayer));
                if (ImGui::MenuItem("Save Scene"))
                {
                } //EnableFlag("CreateSceneWindow");
                if (ImGui::MenuItem("Load Scene"))
                {
                } //EnableFlag("CreateSceneWindow");

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Debug"))
            {
                if (ImGui::MenuItem("Input Analysis"))
                {
                } //ToggleFlag("ShowDebugOverlay");
                if (ImGui::MenuItem("Performance Analysis"))
                {
                    // Todo: menu
                    // ToggleFlag("ShowDebugOverlay");
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Log Window"))  UIUpdate.AddPanel(new EditorLogPanel(_editorLayer));
                if (ImGui::MenuItem("Hierarchy Window"))  UIUpdate.AddPanel(new SceneHierarchyPanel(_editorLayer));
                if (ImGui::MenuItem("Inspector Window"))  UIUpdate.AddPanel(new InspectorPanel(_editorLayer));

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
        ImGui::PopStyleColor();

        return UIUpdate;
    }
}
