#include "anepch.h"
#include "ViewportPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "ANE/Core/Application.h"
#include "ANE/Core/Window.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Core/Layers/EditorLayer.h"
#include "ANE/Core/Scene/Components/TransformComponent.h"
#include "ANE/Input/EditorInputSystem.h"
#include "ANE/Input/Input.h"
#include "ANE/Physics/Physics.h"
#include "ANE/Utilities/AneImGui.h"
#include "ANE/Utilities/Gizmo.h"
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

            DrawGizmos();
        }
        if(!open) ImGui::PopStyleVar();
        ImGui::End();

        return uiUpdate;
    }

    void ViewportPanel::ViewMenu()
    {
        ViewTransformMenu();
        ViewPhysicsDebugMenu();
    }

    void ViewportPanel::ViewTransformMenu()
    {
        if (ImGui::BeginMenu("Transform"))
        {
            const char* spaceOptions[2] { "World", "Local" };
            int currentSpace = _transformSpace;

            if (AneImGui::LabelCombo("Space", &currentSpace, spaceOptions, 2))
            {
                _transformSpace = static_cast<ImGuizmo::MODE>(currentSpace);
            }

            const char* operationOptions[4] { "All", "Translate", "Rotate", "Scale" };
            int currentOperation = _transformOperation;

            if (AneImGui::LabelCombo("Operation", &currentOperation, operationOptions, 4))
            {
                switch (currentOperation)
                {
                    case 0: _transformOperation = ImGuizmo::UNIVERSAL; break;
                    case 1: _transformOperation = ImGuizmo::TRANSLATE; break;
                    case 2: _transformOperation = ImGuizmo::ROTATE; break;
                    case 3: _transformOperation = ImGuizmo::SCALE; break;
                }
            }

            AneImGui::LabelCheckbox("Snap", &_useSnap);

            ImGui::BeginDisabled(!_useSnap);
            if(AneImGui::LabelDragFloat("Snap Value", &_snapValues.X, .1f, 0, FLT_MAX))
            {
                _snapValues = _snapValues.X;
            }
            if(AneImGui::LabelDragFloat("Rotate Snap Value", &_snapRotationValues.X, 5.f, 0, FLT_MAX))
            {
                _snapRotationValues = _snapRotationValues.X;
            }
            ImGui::EndDisabled();

            ImGui::EndMenu();
        }
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

    void ViewportPanel::DrawGizmos()
    {
        ImGuizmo::SetDrawlist();

        const EditorInputSystem& input = GetEditorInputSystem();

        if (input.GetKeyTriggerState(KeyCodeT) == TriggerStarted)
            _transformOperation = _transformOperation == ImGuizmo::TRANSLATE ? ImGuizmo::UNIVERSAL : ImGuizmo::TRANSLATE;
        if (input.GetKeyTriggerState(KeyCodeE) == TriggerStarted)
            _transformOperation = _transformOperation == ImGuizmo::ROTATE ? ImGuizmo::UNIVERSAL : ImGuizmo::ROTATE;
        if (input.GetKeyTriggerState(KeyCodeR) == TriggerStarted)
            _transformOperation = _transformOperation == ImGuizmo::SCALE ? ImGuizmo::UNIVERSAL : ImGuizmo::SCALE;

        if(input.GetKeyTriggerState(KeyCodeLShift) == TriggerStarted)
            _useSnap = !_useSnap;

        if (const std::vector<std::string>* selectedEntityUUIDS = SelectionManager::GetSelection(SelectionManager::UI); !selectedEntityUUIDS->empty())
        {
            Entity selectedEntity = _editorLayer->GetActiveScene()->GetEntityWithUUID(selectedEntityUUIDS->at(0));
            if(!selectedEntity.HasComponent<TransformComponent>()) return;

            Gizmos::PushTransformSpace(_transformSpace);
            Gizmos::PushTransformOperation(_transformOperation);
            Gizmos::PushUseSnap(_useSnap);
            Gizmos::PushSnapValue(_transformOperation == ImGuizmo::ROTATE ? _snapRotationValues : _snapValues);

            Gizmos::TransformHandle(&selectedEntity.GetComponent<TransformComponent>().Transform);

            Gizmos::PopStyle();
        }
    }
}
