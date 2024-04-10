#include "anepch.h"
#include "CameraComponent.h"

namespace Engine
{
    void CameraComponent::SetPosition(const Vector3 newPosition)
    {
        Transform.SetPosition(newPosition);
        UpdateViewMatrix();
    }

    void CameraComponent::SetRotation(const Vector3 newRotation)
    {
        Transform.SetRotation(newRotation);
        UpdateViewMatrix();
    }

    void CameraComponent::SetPerspective(const float fov, const float aspect, const float zNear, const float zFar)
    {
        const Matrix4x4 currentMatrix = PerspectiveMatrix;
        _fieldOfView = fov;
        _zNear = zNear;
        _zFar = zFar;
        PerspectiveMatrix = Matrix4x4::Convert(glm::perspective(glm::radians(_fieldOfView), aspect, _zNear, _zFar));
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
        const Matrix4x4 currentMatrix = PerspectiveMatrix;
        PerspectiveMatrix = Matrix4x4::Convert(glm::perspective(glm::radians(_fieldOfView), aspect, _zNear, _zFar));
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
        const Matrix4x4 currentMatrix = ViewMatrix;

        ViewMatrix = Transform;

        if (_flipY) {
            //flip y coord
            ViewMatrix[3][1] *= -1.f;
            //flip y axis
            ViewMatrix[1][1] *= -1.f;
        }

        ViewPos = Vector4(Transform.GetPosition(), 0.0f) * Vector4(-1.0f, 1.0f, -1.0f, 1.0f);

        if (ViewMatrix != currentMatrix) {
            _updated = true;
        }
    }
}
