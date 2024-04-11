#pragma once
#include "Matrix4x4.h"
#include "ANE/Core/Math/Quaternion.h"
#include "ANE/Core/Math/Vector3.h"

namespace Engine
{
    struct TransformMatrix
    {
    public:
        TransformMatrix(const Matrix4x4& transform = Matrix4x4::Identity()) : _transformMatrix(transform) {}
        TransformMatrix(const TransformMatrix&) = default;

        void SetPosition(const Vector3 newPosition)
        {
            _transformMatrix[3] = Vector4(newPosition, 1);
        }

        /**
         * Adds a positional offset to the Transform along the coordinate system of the world.
         * @param delta World space offset
         */
        void AddPosition(const Vector3 delta)
        {
            _transformMatrix.Translate((Matrix3x3)_transformMatrix.GetInverse() * delta);
        }

        void SetRotation(const Vector3 newRotation, const bool isDegrees = false)
        {
            _transformMatrix.SetRotation(newRotation, isDegrees);
        }

        Vector3 GetPosition() const
        {
            return _transformMatrix.GetPosition();
        }

        void SetRotation(const Quaternion newRotation)
        {
            _transformMatrix.SetRotation(newRotation);
        }

        void AddRotation(const Vector3 delta, const bool isDegrees = false)
        {
            _transformMatrix.Rotate(delta, isDegrees);
        }

        void AddRotation(const Quaternion delta)
        {
            _transformMatrix.Rotate(delta);
        }

        Vector3 GetEulerAngles(const bool isDegrees = false) const
        {
            return _transformMatrix.GetEulerAngles(isDegrees);
        }

        Quaternion GetQuaternion() const
        {
            return _transformMatrix.GetQuaternion();
        }

        void Scale(const Vector3 scale)
        {
            _transformMatrix.Scale(scale);
        }

        Vector3 GetScale() const
        {
            return _transformMatrix.GetScale();
        }

        operator const Matrix4x4&() const { return _transformMatrix; }
        operator Matrix4x4&() { return _transformMatrix; }

    private:
        Matrix4x4 _transformMatrix;
    };
}
