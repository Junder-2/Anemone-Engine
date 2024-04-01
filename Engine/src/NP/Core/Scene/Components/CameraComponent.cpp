#include "nppch.h"
#include "CameraComponent.h"

namespace Engine
{
    void CameraComponent::SetPosition(const glm::vec3 position)
    {
        Transform[3] = xyz1(position);
        UpdateViewMatrix();
    }

    void CameraComponent::SetRotation(const glm::vec3 rotation)
    {
        Transform[0] = glm::vec4(1, 0, 0, 0);
        Transform[1] = glm::vec4(0, 1, 0, 0);
        Transform[2] = glm::vec4(0, 0, 1, 0);
        Transform = rotate(Transform, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
        Transform = rotate(Transform, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
        Transform = rotate(Transform, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));

        UpdateViewMatrix();
    }

    void CameraComponent::SetPerspective(const float fov, const float aspect, const float zNear, const float zFar)
    {
        const glm::mat4 currentMatrix = PerspectiveMatrix;
        _fieldOfView = fov;
        _zNear = zNear;
        _zFar = zFar;
        PerspectiveMatrix = glm::perspective(glm::radians(_fieldOfView), aspect, _zNear, _zFar);
        if (_flipY) {
            //flip y axis
            PerspectiveMatrix[1][1] *= -1.0f;
        }
        if (ViewMatrix != currentMatrix) {
            _updated = true;
        }
    }

    void CameraComponent::UpdateAspectRatio(const float aspect)
    {
        const glm::mat4 currentMatrix = PerspectiveMatrix;
        PerspectiveMatrix = glm::perspective(glm::radians(_fieldOfView), aspect, _zNear, _zFar);
        if (_flipY) {
            //flip y axis
            PerspectiveMatrix[1][1] *= -1.0f;
        }
        if (ViewMatrix != currentMatrix) {
            _updated = true;
        }
    }

    void CameraComponent::UpdateViewMatrix()
    {
        const glm::mat4 currentMatrix = ViewMatrix;

        ViewMatrix = Transform;

        if (_flipY) {
            //flip y coord
            ViewMatrix[3][1] *= -1.f;
            //flip y axis
            ViewMatrix[1][1] *= -1.f;
        }

        ViewPos = glm::vec4(GetPosition(), 0.0f) * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);

        if (ViewMatrix != currentMatrix) {
            _updated = true;
        }
    }
}
