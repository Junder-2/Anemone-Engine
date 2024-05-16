#include "anepch.h"
#include "ViewportPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "ANE/Core/Application.h"
#include "ANE/Core/Window.h"
#include "ANE/Physics/Physics.h"
#include "Platform/Vulkan/VulkanRenderer.h"

namespace Engine
{
    ViewportPanel::ViewportPanel(EditorLayer* editorLayer)
    {
        _editorLayer = editorLayer;
    }

    ViewportPanel::~ViewportPanel()
    {
        Application::Get().GetWindow().RemoveViewport(ImHashStr("Viewport"));
    }

    UIUpdateWrapper ViewportPanel::OnPanelRender()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        UIUpdateWrapper uiUpdate;

        const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar;

        ImGui::SetNextWindowSizeConstraints(Vector2(100.f, 100.f), Vector2(FLT_MAX, FLT_MAX));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        const bool open = ImGui::Begin("Viewport", 0, flags);
        if (open)
        {
            if(open) ImGui::PopStyleVar();
            if (!_initialized)
            {
                Application::Get().GetWindow().SetActiveViewport(ImGui::GetCurrentWindow()->ID);
                _initialized = true;
            }

            if (ImGui::BeginMenuBar())
            {
                ViewMenu();

                ImGui::EndMenuBar();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
            ImGui::Image(Vulkan::VulkanRenderer::GetImGuiViewportSet(), ImGui::GetContentRegionAvail());
            ImGui::PopStyleVar();
        }
        if(!open) ImGui::PopStyleVar();
        ImGui::End();

        return uiUpdate;
    }

    void ViewportPanel::ViewMenu()
    {
        ViewPhysicsDebugMenu();
    }

    void ViewportPanel::ViewPhysicsDebugMenu()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        #ifndef ANE_DIST
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
        #endif
    }
}
