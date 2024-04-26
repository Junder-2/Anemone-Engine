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
            .data<&CameraComponent::_zNear>("float"_hs).prop("display_name"_hs, "Z-Near");


                /*.data<&CameraComponent::_fieldOfView>("FieldOfView"_hs).prop("display_name"_hs, "Field Of View")
            .data<&CameraComponent::Transform>("Transform"_hs).prop("display_name"_hs, "Transform");
                .data<&CameraComponent::ViewPos>("ViewPos"_hs).prop("display_name"_hs, "View Position");
*/
        }
    private:
        float _fieldOfView = 60;
        float _zNear = 0.1f, _zFar = 1000.f;

        bool _updated = true;
        bool _flipY = true;

        void UpdateViewMatrix();
    };
}
