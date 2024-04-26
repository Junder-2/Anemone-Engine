#include "anepch.h"
#include "CameraComponent.h"

#include "glm/ext/matrix_clip_space.hpp"

namespace Engine
{
    void CameraComponent::SetFOV(const float fov)
    {
        _fieldOfView = fov;

        UpdateProjectionMatrix();
    }

    void CameraComponent::SetPerspective(const float fov, const float aspect, const float zNear, const float zFar)
    {
        _aspectRatio = aspect;
        _fieldOfView = fov;
        _zNear = zNear;
        _zFar = zFar;

        UpdateProjectionMatrix();
    }

    void CameraComponent::SetAspectRatio(const float aspect)
    {
        _aspectRatio = aspect;

        UpdateProjectionMatrix();
    }

    void CameraComponent::UpdateProjectionMatrix()
    {
        PerspectiveMatrix = Matrix4x4::Convert(glm::perspective(glm::radians(_fieldOfView), _aspectRatio, _zNear, _zFar));
        if (_flipY) {
            //flip y axis
            PerspectiveMatrix[1][1] *= -1.0f;
        }
    }
}
