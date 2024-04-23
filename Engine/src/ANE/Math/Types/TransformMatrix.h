#pragma once
#include "Matrix4x4.h"
#include "ANE/Math/Types/Quaternion.h"
#include "ANE/Math/Types/Vector3.h"

namespace Engine
{
    struct TransformMatrix
    {
    public:
        TransformMatrix(const Matrix4x4& transform = Matrix4x4::Identity()) : _localToWorld(transform) {}
        TransformMatrix(const TransformMatrix&) = default;

        /**
         * Sets the position
         * @param newPosition World space position
         */
        void SetPosition(const Vector3 newPosition)
        {
            _localToWorld[3] = Vector4(newPosition, 1);
        }

        /**
         * Adds a positional offset to the Transform
         * @param delta World space offset
         */
        void AddPosition(const Vector3 delta)
        {
            _localToWorld.AddPosition(delta);
        }

        /**
         * Adds a positional offset to the Transform
         * @param delta Local space offset
         */
        void AddLocalPosition(const Vector3 delta)
        {
            _localToWorld.Translate(delta);
        }

        void SetRotation(const Vector3 newRotation, const bool isDegrees = false)
        {
            _localToWorld.SetRotation(newRotation, isDegrees);
        }

        Vector3 GetPosition() const
        {
            return _localToWorld.GetPosition();
        }

        void SetRotation(const Quaternion newRotation)
        {
            _localToWorld.SetRotation(newRotation);
        }

        //TODO: Add rotation are not properly working
        void AddRotation(const Vector3 delta, const bool isDegrees = false)
        {
            _localToWorld.SetRotation(delta, isDegrees);
        }

        void AddRotation(const Quaternion delta)
        {
            _localToWorld.Rotate(delta);
        }

        Vector3 GetEulerAngles(const bool isDegrees = false) const
        {
            return _localToWorld.GetEulerAngles(isDegrees);
        }

        Quaternion GetQuaternion() const
        {
            return _localToWorld.GetQuaternion();
        }

        void Scale(const Vector3 scale)
        {
            _localToWorld.Scale(scale);
        }

        Vector3 GetScale() const
        {
            return _localToWorld.GetScale();
        }

        Matrix4x4 GetWorldToLocal() const
        {
            return _localToWorld.GetInverse();
        }

        Matrix4x4 GetLocalToWorld() const
        {
            return _localToWorld;
        }

        Vector3 GetRight() const
        {
            return _localToWorld.GetRight();
        }

        Vector3 GetUp() const
        {
            return _localToWorld.GetUp();
        }

        Vector3 GetForward() const
        {
            return _localToWorld.GetForward();
        }

    private:
        Matrix4x4 _localToWorld;
    };
}
