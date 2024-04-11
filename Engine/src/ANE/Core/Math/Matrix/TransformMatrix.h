#pragma once
#include "Matrix4x4.h"
#include "ANE/Core/Math/Quaternion.h"
#include "ANE/Core/Math/Vector3.h"

namespace Engine
{
    struct TransformMatrix
    {
    public:
        TransformMatrix(const Matrix4x4& transform = Matrix4x4(1.f)) : _transformMatrix(transform) {}
        TransformMatrix(const TransformMatrix&) = default;

        void SetPosition(const Vector3 newPosition)
        {
            _transformMatrix[3] = Vector4(newPosition, 1);
        }

        void AddPosition(const Vector3 delta)
        {
            _transformMatrix.Translate(delta);
        }

        Vector3 GetPosition() const
        {
            return {_transformMatrix[3][0], _transformMatrix[3][1], _transformMatrix[3][2]};
        }

        void SetRotation(const Vector3 newRotation) // todo: fix, overwrite rotation
        {
            const Vector3 scale = GetScale();

            _transformMatrix[0] = Vector4(1, 0, 0, 0) * scale.X;
            _transformMatrix[1] = Vector4(0, 1, 0, 0) * scale.Y;
            _transformMatrix[2] = Vector4(0, 0, 1, 0) * scale.Z;
            AddRotation(newRotation);
        }

        void SetRotation(const Quaternion newRotation)
        {
            const Vector3 scale = GetScale();

            Matrix3x3 rotMatrix = (newRotation).GetMatrix();

            _transformMatrix[0] = Vector4(rotMatrix[0], 0) * scale.X;
            _transformMatrix[1] = Vector4(rotMatrix[1], 0) * scale.Y;
            _transformMatrix[2] = Vector4(rotMatrix[2], 0) * scale.Z;
        }

        void AddRotation(const Vector3 delta)
        {
            _transformMatrix.Rotate(glm::radians(delta.X), Vector3::RightVector());
            _transformMatrix.Rotate(glm::radians(delta.Y), Vector3::UpVector());
            _transformMatrix.Rotate(glm::radians(delta.Z), Vector3::ForwardVector());
        }

        void AddRotation(const Quaternion delta)
        {
            Matrix4x4 rotMatrix = (delta).GetMatrix();
            rotMatrix[3][3] = 1;

            _transformMatrix *= rotMatrix;
        }

        Vector3 GetEulerRotation() const
        {
            const Vector3 euler = Quaternion(_transformMatrix).GetEulerAngles();
            return {euler.X, euler.Y, euler.Z};
        }

        Quaternion GetRotation() const
        {
            return Quaternion(_transformMatrix);
        }

        Vector3 GetScale() const
        {
            const Vector3 scale(Vector3(_transformMatrix[0]).Length(), Vector3(_transformMatrix[1]).Length(), Vector3(_transformMatrix[2]).Length());
            return scale;
        }

        operator const Matrix4x4&() const { return _transformMatrix; }
        operator Matrix4x4&() { return _transformMatrix; }

    private:
        Matrix4x4 _transformMatrix;
    };
}
