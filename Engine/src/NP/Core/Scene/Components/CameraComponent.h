#pragma once
#include "../../TransformMatrix.h"

namespace Engine
{
    struct CameraComponent
    {
    public:
        TransformMatrix Transform {};
        glm::vec4 ViewPos = {};
        glm::mat4 PerspectiveMatrix = {};
        glm::mat4 ViewMatrix = {};

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

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
