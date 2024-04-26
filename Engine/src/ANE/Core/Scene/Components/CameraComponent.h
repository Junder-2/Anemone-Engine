#pragma once
#include "Component.h"
#include "ANE/Math/Types/TransformMatrix.h"
#include "ANE/Utilities/MetaUtilities.h"

namespace Engine
{
    struct CameraComponent : Component // this might need to be remade..
    {
    public:
        TransformMatrix Transform {};
        Vector4 ViewPos = {};
        Matrix4x4 PerspectiveMatrix = {};
        Matrix4x4 ViewMatrix = {};

        ANE_COMPONENT_INIT(CameraComponent)
        // CameraComponent() : Component(typeid(*this).name()) {} // does this compile?  // NOLINT(modernize-use-equals-default)
        // CameraComponent(const CameraComponent&) = default;


        void SetPerspective(float fov, float aspect, float zNear, float zFar);

        void UpdateAspectRatio(float aspect);

        void SetPosition(Vector3 newPosition);
        void SetRotation(Vector3 newRotation);

        float GetFOV() const
        {
            return _fieldOfView;
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
            .data<&CameraComponent::ViewPos>("Vector4"_hs).prop("display_name"_hs, "View Position");
            /*
            .data<&CameraComponent::Transform>("Transform"_hs).prop("display_name"_hs, "Transform")
            .prop("Position"_hs, "Position")
             .prop("Rotation"_hs, "Rotation")
             .prop("Scale"_hs, "Scale");
             */
        }
/*
        operator const TransformMatrix&() const { return Transform; }
        operator TransformMatrix&() { return Transform; }*/
    private:
        float _fieldOfView = 60;
        float _zNear = 1;
        float _zFar = 1000;

        bool _updated = true;
        bool _flipY = true;


        void UpdateViewMatrix();
    };
}
