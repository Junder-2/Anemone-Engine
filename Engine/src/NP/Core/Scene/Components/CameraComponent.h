#pragma once
#include "Component.h"
#include "../../TransformMatrix.h"

namespace Engine
{
    struct CameraComponent : Component // this might need to be remade..
    {
    public:
        TransformMatrix Transform {};
        glm::vec4 ViewPos = {};
        glm::mat4 PerspectiveMatrix = {};
        glm::mat4 ViewMatrix = {};


        NP_COMPONENT_INIT(CameraComponent)
        // CameraComponent() : Component(typeid(*this).name()) {} // does this compile?  // NOLINT(modernize-use-equals-default)
        // CameraComponent(const CameraComponent&) = default;


        void SetPerspective(float fov, float aspect, float zNear, float zFar);

        void UpdateAspectRatio(float aspect);

        void SetPosition(glm::vec3 newPosition);
        void SetRotation(glm::vec3 newRotation);

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

    private:
        float _fieldOfView = 60;
        float _zNear = 0.1f, _zFar = 1000.f;

        bool _updated = true;
        bool _flipY = false;

        void UpdateViewMatrix();
    };
}
