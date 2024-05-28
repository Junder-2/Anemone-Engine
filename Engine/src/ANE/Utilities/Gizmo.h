#pragma once
#include "ANE/Math/Types/Vector3.h"
#include <ImGuizmo.h>

namespace Engine
{
    struct TransformMatrix;
}

namespace Engine::Gizmos
{
    struct GizmoContext
    {
        bool GlobalUseSnap = false;
        Vector3 GlobalSnapValues = .1f;

        bool UseSnap = false;
        Vector3 SnapValues = .1f;
        ImGuizmo::MODE TransformSpace = ImGuizmo::LOCAL;
        ImGuizmo::OPERATION TransformOperation = ImGuizmo::UNIVERSAL;
    };

    extern GizmoContext* _gizmoContext;

    GizmoContext* Initialize();
    GizmoContext* GetContext();
    void Cleanup();

    void SetGlobalUseSnap(bool enable);
    void SetGlobalSnapValues(Vector3 values);

    void PushUseSnap(bool enable);
    void PushSnapValue(Vector3 values);
    void PushTransformSpace(ImGuizmo::MODE mode);
    void PushTransformOperation(ImGuizmo::OPERATION operation);

    void TransformHandle(TransformMatrix* transform, bool editable = true);

    void ResetStyle();
    void PopStyle();
}
