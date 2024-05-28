#include "anepch.h"
#include "Gizmo.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include "ANE/Core/Scene/Components/CameraComponent.h"
#include "ANE/Math/MatrixTypes.h"
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

    void PushUseSnap(const bool enable)
    {
        _gizmoContext->UseSnap = enable;
    }

    void PushSnapValue(const Vector3 values)
    {
        _gizmoContext->SnapValues = values;
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
        //cameraView[1][1] *= -1.f;

        Matrix4x4 transformMatrix = transform->GetLocalToWorld();

        ImGuizmo::Enable(editable);

        ImGuizmo::Manipulate(&(cameraView[0].X), &(cameraPerspective[0].X),
            _gizmoContext->TransformOperation, _gizmoContext->TransformSpace,
            &(transformMatrix[0].X), nullptr, _gizmoContext->UseSnap ? &_gizmoContext->SnapValues.X : nullptr);

        if (ImGuizmo::IsUsing() && editable)
        {
            transform->SetPosition(transformMatrix.GetPosition());
            transform->SetRotation(transformMatrix.GetQuaternion());
            transform->SetScale(transformMatrix.GetScale());
        }
    }

    void ResetStyle()
    {
        _gizmoContext->GlobalUseSnap = false;
        _gizmoContext->GlobalSnapValues = .1f;
        PopStyle();
    }

    void PopStyle()
    {
        _gizmoContext->UseSnap = _gizmoContext->GlobalUseSnap;
        _gizmoContext->SnapValues = _gizmoContext->GlobalSnapValues;

        _gizmoContext->TransformOperation = ImGuizmo::UNIVERSAL;
        _gizmoContext->TransformSpace = ImGuizmo::LOCAL;
    }
}
