#pragma once
#include "Matrix4x4.h"
#include "ANE/Core/Math/Quaternion.h"
#include "ANE/Core/Math/Vector3.h"

namespace Engine
{
    struct TransformMatrix
    {
    public:
        TransformMatrix(const Matrix4x4& transform = Matrix4x4::Identity()) : _worldToLocal(transform) {}
        TransformMatrix(const TransformMatrix&) = default;

        /**
         * Sets the position
         * @param newPosition World space position
         */
        void SetPosition(const Vector3 newPosition)
        {
            _worldToLocal[3] = Vector4(newPosition, 1);
        }

        /**
         * Adds a positional offset to the Transform
         * @param delta World space offset
         */
        void AddPosition(const Vector3 delta)
        {
            _worldToLocal.AddPosition(delta);
        }

        /**
         * Adds a positional offset to the Transform
         * @param delta Local space offset
         */
        void AddLocalPosition(const Vector3 delta)
        {
            _worldToLocal.Translate(delta);
        }

        void SetRotation(const Vector3 newRotation, const bool isDegrees = false)
        {
            _worldToLocal.SetRotation(newRotation, isDegrees);
        }

        Vector3 GetPosition() const
        {
            return _worldToLocal.GetPosition();
        }

        void SetRotation(const Quaternion newRotation)
        {
            _worldToLocal.SetRotation(newRotation);
        }

        //TODO: Add rotation are not properly working
        void AddRotation(const Vector3 delta, const bool isDegrees = false)
        {
            _worldToLocal.SetRotation(delta, isDegrees);
        }

        void AddRotation(const Quaternion delta)
        {
            _worldToLocal.Rotate(delta);
        }

        Vector3 GetEulerAngles(const bool isDegrees = false) const
        {
            return _worldToLocal.GetEulerAngles(isDegrees);
        }

        Quaternion GetQuaternion() const
        {
            return _worldToLocal.GetQuaternion();
        }

        void Scale(const Vector3 scale)
        {
            _worldToLocal.Scale(scale);
        }

        Vector3 GetScale() const
        {
            return _worldToLocal.GetScale();
        }

        Matrix4x4 GetWorldToLocal() const
        {
            return _worldToLocal;
        }

        Matrix4x4 GetLocalToWorld() const
        {
            return _worldToLocal.GetInverse();
        }

        Vector3 GetRight() const
        {
            return _worldToLocal.GetRight();
        }

        Vector3 GetUp() const
        {
            return _worldToLocal.GetUp();
        }

        Vector3 GetForward() const
        {
            return _worldToLocal.GetForward();
        }

    private:
        Matrix4x4 _worldToLocal;
    };
}
