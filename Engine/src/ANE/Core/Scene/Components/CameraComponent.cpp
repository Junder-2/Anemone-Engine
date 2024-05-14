#include "anepch.h"
#include "CameraComponent.h"

#include "ANE/Renderer/Camera.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace Engine
{
    void CameraComponent::SetFOV(const float fov)
    {
        _fieldOfView = fov;
        UpdateProjectionMatrix();
    }

    void CameraComponent::SetAspectRatio(const float aspect)
    {
        _aspectRatio = aspect;
        UpdateProjectionMatrix();
    }

    void CameraComponent::SetNearClip(const float nearClip)
    {
        _zNear = nearClip;
        UpdateProjectionMatrix();
    }

    void CameraComponent::SetFarClip(const float farClip)
    {
        _zFar = farClip;
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

    void CameraComponent::SetPriority(const int newValue)
    {
        _priority = newValue;
        Camera::MarkPriorityDirty();
    }

    void CameraComponent::UpdateProjectionMatrix()
    {

        _perspectiveMatrix = Matrix4x4::Perspective(_fieldOfView * FMath::DEGREES_TO_RAD, _aspectRatio, _zFar, _zNear); //Reverse-Z
        _perspectiveMatrix[1][1] *= -1.0f; //Flip Y to match render matrix layout

        _dirty = true;
    }
}
