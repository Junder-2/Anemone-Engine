#pragma once
#include "ANE/Math/Types/Vector3.h"
#include <ImGuizmo.h>

namespace Engine
{
    struct TransformMatrix;
}

namespace Engine::Gizmos
{
    struct GizmoContext // TODO replace pushing methods with one similar to ImGui
    {
        bool GlobalUseSnap; // = false
        Vector3 GlobalSnapValues; // = .1f
        Vector3 GlobalRotationSnapValues; // = 45.f

        bool UseSnap; // = false
        Vector3 SnapValues; // = .1f
        Vector3 RotationSnapValues; // = 45.f
        ImGuizmo::MODE TransformSpace; // = ImGuizmo::LOCAL
        ImGuizmo::OPERATION TransformOperation; // = ImGuizmo::UNIVERSAL
    };

    extern GizmoContext* _gizmoContext;

    GizmoContext* Initialize();
    GizmoContext* GetContext();
    void Cleanup();

    /** The default value use snap will use */
    void SetGlobalUseSnap(bool enable);
    /** The default values snapping will use */
    void SetGlobalSnapValues(Vector3 values);
    /** The default values rotation snapping will use */
    void SetGlobalRotationSnapValues(Vector3 values);

    /** Override next use snap value */
    void PushUseSnap(bool enable);
    /** Override next snapping values */
    void PushSnapValue(Vector3 values);
    /** Override next rotation snapping values */
    void PushRotationSnapValue(Vector3 values);
    /** Override next transform space value */
    void PushTransformSpace(ImGuizmo::MODE mode);
    /** Override next transform operation value */
    void PushTransformOperation(ImGuizmo::OPERATION operation);

    /** Draws a transform handle using a TransformMatrix */
    void TransformHandle(TransformMatrix* transform, bool editable = true);

    /** Sets all style values to default */
    void ResetStyle();
    /** Reverts pushed values back to default */
    void PopStyle();
}
