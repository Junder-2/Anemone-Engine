#include "anepch.h"
#include "Gizmo.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include "ColorUtilities.h"
#include "ANE/Core/Scene/Components/CameraComponent.h"
#include "ANE/Math/MatrixTypes.h"
#include "ANE/Math/VMath.h"
#include "ANE/Renderer/Camera.h"

namespace Engine::Gizmos
{
    GizmoContext* _gizmoContext = nullptr;

    GizmoContext* Initialize()
    {
        _gizmoContext = new GizmoContext();

        ResetStyle();
        return _gizmoContext;
    }

    GizmoContext* GetContext()
    {
        return _gizmoContext;
    }

    void Cleanup()
    {
        delete _gizmoContext;
    }

    void SetGlobalUseSnap(const bool enable)
    {
        _gizmoContext->GlobalUseSnap = enable;
    }

    void SetGlobalSnapValues(const Vector3 values)
    {
        _gizmoContext->GlobalSnapValues = values;
    }

    void SetGlobalRotationSnapValues(const Vector3 values)
    {
        _gizmoContext->GlobalRotationSnapValues = values;
    }

    void PushUseSnap(const bool enable)
    {
        _gizmoContext->UseSnap = enable;
    }

    void PushSnapValue(const Vector3 values)
    {
        _gizmoContext->SnapValues = values;
    }

    void PushRotationSnapValue(const Vector3 values)
    {
        _gizmoContext->RotationSnapValues = values;
    }

    void PushTransformSpace(const ImGuizmo::MODE mode)
    {
        _gizmoContext->TransformSpace = mode;
    }

    void PushTransformOperation(const ImGuizmo::OPERATION operation)
    {
        _gizmoContext->TransformOperation = operation;
    }

    void TransformHandle(TransformMatrix* transform, const bool editable)
    {
        ImGuizmo::SetID(0);

        if(!Camera::HasCamera())
        {
            ANE_ELOG_WARN("No camera found cant draw transform handle");
        }
        Matrix4x4 cameraPerspective = Camera::GetCameraComponent().GetPerspectiveMatrix();
        cameraPerspective[1][1] *= -1.f;
        Matrix4x4 cameraView = Camera::GetCameraTransform().GetWorldToLocal();

        Matrix4x4 transformMatrix = transform->GetLocalToWorld();
        Matrix4x4 deltaTransformMatrix;

        ImGuizmo::Enable(editable);

        ImGuizmo::Manipulate(
            &(cameraView[0].X), &(cameraPerspective[0].X),
            _gizmoContext->TransformOperation, _gizmoContext->TransformSpace,
            &(transformMatrix[0].X), &(deltaTransformMatrix[0].X),
            _gizmoContext->UseSnap ? &_gizmoContext->SnapValues.X : nullptr,
            _gizmoContext->UseSnap ? &_gizmoContext->RotationSnapValues.X : nullptr);

        if (ImGuizmo::IsUsing() && editable && deltaTransformMatrix != Matrix4x4::Identity())
        {
            transform->SetScale(transformMatrix.GetAbsoluteScale() * Math::Sign(transform->GetScale()));
            transform->SetRotation(transformMatrix.GetQuaternion());
            transform->SetPosition(transformMatrix.GetPosition());
        }
    }

    void ResetStyle()
    {
        auto* style = &ImGuizmo::GetStyle();

        style->TranslationLineThickness   = 3.0f; // 3.0f
        style->TranslationLineArrowSize   = 7.0f; // 6.0f
        style->RotationLineThickness      = 2.0f; // 2.0f
        style->RotationOuterLineThickness = 3.0f; // 3.0f
        style->ScaleLineThickness         = 3.0f; // 3.0f
        style->ScaleLineCircleSize        = 10.0f; // 6.0f
        style->HatchedAxisLineThickness   = 0.0f; // 6.0f
        style->CenterCircleSize           = 5.0f; // 6.0f
        style->CenterCircleThickness      = 2.5f; // 3.0f

        style->Colors[ImGuizmo::DIRECTION_X] = Vector4(ColorUtilities::HexToRGB(0xC80030), .95f);
        style->Colors[ImGuizmo::DIRECTION_Y] = Vector4(ColorUtilities::HexToRGB(0x00C830), .95f);
        style->Colors[ImGuizmo::DIRECTION_Z] = Vector4(ColorUtilities::HexToRGB(0x0030C8), .95f);

        _gizmoContext->GlobalUseSnap = false;
        _gizmoContext->GlobalSnapValues = .1f;
        _gizmoContext->GlobalRotationSnapValues = 45.f;
        PopStyle();
    }

    void PopStyle()
    {
        _gizmoContext->UseSnap = _gizmoContext->GlobalUseSnap;
        _gizmoContext->SnapValues = _gizmoContext->GlobalSnapValues;
        _gizmoContext->RotationSnapValues = _gizmoContext->GlobalRotationSnapValues;

        _gizmoContext->TransformOperation = ImGuizmo::UNIVERSAL;
        _gizmoContext->TransformSpace = ImGuizmo::LOCAL;
    }
}
