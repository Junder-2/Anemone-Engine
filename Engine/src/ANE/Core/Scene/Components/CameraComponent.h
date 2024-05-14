#pragma once
#include "Component.h"
#include "ANE/Math/Types/Matrix4x4.h"

namespace Engine
{
    struct CameraComponent : Component
    {
        ANE_COMPONENT_INIT(CameraComponent)

        void SetFOV(float fov);
        void SetAspectRatio(float aspect);
        void SetNearClip(float nearClip);
        void SetFarClip(float farClip);

        void SetPerspective(float fov, float aspect, float zNear, float zFar);

        Matrix4x4 GetPerspectiveMatrix() const { return _perspectiveMatrix; }
        float GetFOV() const { return _fieldOfView; }
        float GetAspectRatio() const { return _aspectRatio; }
        float GetNearClip() const { return _zNear; }
        float GetFarClip() const { return _zFar; }


        bool IsDirty() const { return _dirty; }

        void SetPriority(const int newValue);
        int GetPriority() const { return _priority; }

        void OnValidate()
        {
            //ANE_ELOG("Camera recalculating");
            UpdateProjectionMatrix();
        }

        static void RegisterComponentMetaData() //TODO: add min max meta data?
        {
            entt::meta<CameraComponent>()
                .data<&CameraComponent::_zNear>("Z_Near"_hs).prop("display_name"_hs, "Z Near")
                .EDITABLE
                .data<&CameraComponent::_zFar>("Z_Far"_hs).prop("display_name"_hs, "Z Far")
                .EDITABLE
                .data<&CameraComponent::_fieldOfView>("Field Of View"_hs).prop("display_name"_hs, "Field Of View")
                .EDITABLE
                .data<&CameraComponent::_aspectRatio>("Aspect Ratio"_hs).prop("display_name"_hs, "Aspect Ratio")
                .EDITABLE
                .func<&CameraComponent::OnValidate>("OnValidate"_hs);
        }

    private:
        void UpdateProjectionMatrix();
        void ClearDirty() { _dirty = false; }

    private:
        Matrix4x4 _perspectiveMatrix = {};

        float _aspectRatio = 1;
        float _fieldOfView = 60;
        float _zNear = 1;
        float _zFar = 1000;

        int _priority = 100;

        bool _dirty = true;

        friend class Camera;
    };
}
