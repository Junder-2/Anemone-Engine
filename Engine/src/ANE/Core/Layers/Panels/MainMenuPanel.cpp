#include "anepch.h"
#include "MainMenuPanel.h"

#include "CreateScenePanel.h"
#include "EditorLogPanel.h"
#include "imgui.h"
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
                PhysicsAnalysisMenu();

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
        return UIUpdate;
    }

    void MainMenuPanel::PhysicsAnalysisMenu()
    {
        if (ImGui::BeginMenu("Physics Analysis"))
        {

            ImGui::EndMenu();
        }
    }
}
