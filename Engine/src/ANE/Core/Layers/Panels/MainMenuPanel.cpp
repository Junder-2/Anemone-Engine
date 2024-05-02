#include "anepch.h"
#include "MainMenuPanel.h"

#include "CreateScenePanel.h"
#include "EditorLogPanel.h"
#include "imgui.h"
#include "InspectorPanel.h"
#include "SceneHierarchyPanel.h"
#include "ANE/Physics/Physics.h"
#include "ANE/Physics/PhysicsTypes.h"

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
            if (ImGui::BeginMenu("Debug Rendering"))
            {
                bool drawDebug = GetPhysicsSystem().IsDebugRendering();
                if(ImGui::Checkbox("Enable", &drawDebug))
                {
                    GetPhysicsSystem().EnableDebugRendering(drawDebug);
                }
                ImGui::BeginDisabled(!drawDebug);

                float displayAlpha = GetPhysicsSystem().GetDebugDisplayAlpha();
                if(ImGui::SliderFloat("Display Alpha", &displayAlpha, 0.f, 1.f))
                {
                    GetPhysicsSystem().SetDebugDisplayAlpha(displayAlpha);
                }

                ImGui::Spacing();

                bool showAABB = GetPhysicsSystem().IsDebugDisplayFlag(PhysicsDebugDisplayFlag::ColliderAABB);
                if(ImGui::Checkbox("Show AABB", &showAABB))
                {
                    GetPhysicsSystem().EnableDebugFlag(PhysicsDebugDisplayFlag::ColliderAABB, showAABB);
                }
                bool showBroadphaseAABB = GetPhysicsSystem().IsDebugDisplayFlag(PhysicsDebugDisplayFlag::ColliderBroadphaseAABB);
                if(ImGui::Checkbox("Show Broadphase AABB", &showBroadphaseAABB))
                {
                    GetPhysicsSystem().EnableDebugFlag(PhysicsDebugDisplayFlag::ColliderBroadphaseAABB, showBroadphaseAABB);
                }
                bool showCollisionShape = GetPhysicsSystem().IsDebugDisplayFlag(PhysicsDebugDisplayFlag::CollisionShape);
                if(ImGui::Checkbox("Show Colliders", &showCollisionShape))
                {
                    GetPhysicsSystem().EnableDebugFlag(PhysicsDebugDisplayFlag::CollisionShape, showCollisionShape);
                }
                bool showShapeNormal = GetPhysicsSystem().IsDebugDisplayFlag(PhysicsDebugDisplayFlag::CollisionShapeNormal);
                if(ImGui::Checkbox("Show Collider Normal", &showShapeNormal))
                {
                    GetPhysicsSystem().EnableDebugFlag(PhysicsDebugDisplayFlag::CollisionShapeNormal, showShapeNormal);
                }
                bool showContactPoint = GetPhysicsSystem().IsDebugDisplayFlag(PhysicsDebugDisplayFlag::ContactPoint);
                if(ImGui::Checkbox("Show Contact Point", &showContactPoint))
                {
                    GetPhysicsSystem().EnableDebugFlag(PhysicsDebugDisplayFlag::ContactPoint, showContactPoint);
                }
                bool showContactNormal = GetPhysicsSystem().IsDebugDisplayFlag(PhysicsDebugDisplayFlag::ContactNormal);
                if(ImGui::Checkbox("Show Contact Normal", &showContactNormal))
                {
                    GetPhysicsSystem().EnableDebugFlag(PhysicsDebugDisplayFlag::ContactNormal, showContactNormal);
                }

                ImGui::EndDisabled();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
    }
}
