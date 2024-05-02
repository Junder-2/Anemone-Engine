#pragma once

#include "Component.h"
#include "ANE/Utilities/MetaUtilities.h"

namespace Engine
{
    struct CameraComponent : Component // this might need to be remade..
    {
        ANE_COMPONENT_INIT(CameraComponent)

        Matrix4x4 PerspectiveMatrix = {};

        void SetFOV(float fov);
        void SetAspectRatio(float aspect);

        void SetPerspective(float fov, float aspect, float zNear, float zFar);

        float GetFOV() const
        {
            return _fieldOfView;
        }

        float GetAspectRatio() const
        {
            return _aspectRatio;
        }

        float GetNearClip() const
        {
            return _zNear;
        }

        float GetFarClip() const
        {
            return _zFar;
        }

        static void RegisterComponentMetaData()
        {
            entt::meta<CameraComponent>()
                .data<&CameraComponent::_zNear>("Z Near"_hs).prop("display_name"_hs, "Z Near")
                .data<&CameraComponent::_zFar>("Z Far"_hs).prop("display_name"_hs, "Z Far")
                .data<&CameraComponent::_fieldOfView>("Field Of View"_hs).prop("display_name"_hs, "Field Of View")
                .EDITABLE;
        }

    private:
        void UpdateProjectionMatrix();

    private:
        float _aspectRatio = 1;
        float _fieldOfView = 60;
        float _zNear = 1;
        float _zFar = 1000;

        bool _updated = true;
        bool _flipY = true;
    };
}
