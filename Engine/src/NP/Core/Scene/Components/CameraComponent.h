#pragma once
#include "glm/gtx/quaternion.hpp"

namespace Engine
{
    struct CameraComponent
    {
    public:
        glm::mat4 Transform {};
        glm::vec4 ViewPos = {};
        glm::mat4 PerspectiveMatrix = {};
        glm::mat4 ViewMatrix = {};

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        CameraComponent(const glm::mat4& transform) : Transform(transform)
        {
        }

        void SetPerspective(float fov, float aspect, float zNear, float zFar);

        void UpdateAspectRatio(float aspect);

        void SetPosition(glm::vec3 position);

        glm::vec3 GetPosition()
        {
            return Transform[3];
        }

        void SetRotation(glm::vec3 rotation);

        glm::vec3 GetEulerRotation() const
        {
            return eulerAngles(toQuat(Transform));
        }

        glm::quat GetRotation() const
        {
            return toQuat(Transform);
        }

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
