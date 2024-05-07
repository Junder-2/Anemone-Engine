#pragma once
#include "Matrix4x4.h"
#include "ANE/Math/Types/Quaternion.h"
#include "ANE/Math/Types/Vector3.h"

namespace Engine
{
    struct TransformMatrix
    {
    public:
        TransformMatrix(const Matrix4x4& transform = Matrix4x4::Identity()) : _localToWorld(transform)
        {
            SyncEulerAngles();
            _scale = _localToWorld.GetScale();
        }
        TransformMatrix(const TransformMatrix&) = default;

        /**
         * Sets the world position of the Transform
         * @param newPosition World space position
         */
        void SetPosition(const Vector3 newPosition)
        {
            _localToWorld[3] = Vector4(newPosition, 1);
            MarkDirty();
        }

        /**
         * Adds a positional offset to the Transform
         * @param delta World space offset
         */
        void AddPosition(const Vector3 delta)
        {
            _localToWorld.AddPosition(delta);
            MarkDirty();
        }

        /**
         * Adds a positional offset to the Transform
         * @param delta Local space offset
         */
        void AddLocalPosition(const Vector3 delta)
        {
            _localToWorld.Translate(delta);
            MarkDirty();
        }

        /**
         * Returns the world position of the Transform
         */
        Vector3 GetPosition() const
        {
            return _localToWorld.GetPosition();
        }

        /**
         * Sets the rotation of the Transform in Quaternion
         * @param newRotation World space rotation
         */
        void SetRotation(const Quaternion& newRotation)
        {
            _localToWorld.SetRotation(newRotation);
            SyncEulerAngles();
            MarkDirty();
        }

        /**
         * Adds to the rotation of the Transform in Quaternion
         * @param delta World space offset
         */
        void AddRotation(const Quaternion& delta)
        {
            _localToWorld.Rotate(delta);
            SyncEulerAngles();
            MarkDirty();
        }

        /**
         * Returns the rotation of the Transform in Quaternion
         */
        Quaternion GetQuaternion() const
        {
            return _localToWorld.GetQuaternion();
        }

        /**
         * Sets the rotation of the Transform using EulerAngles
         * @param newRotation World space rotation
         * @param isDegrees Input is in degrees
         */
        void SetRotation(const Vector3 newRotation, const bool isDegrees = false)
        {
            _eulerAngles = newRotation * (isDegrees ? FMath::DEGREES_TO_RAD : 1.f);
            _localToWorld.SetRotation(_eulerAngles, false);
            MarkDirty();
        }

        /**
         * Add to the rotation of the Transform in EulerAngles
         * @param delta World space offset
         * @param isDegrees input is in degrees
         */
        void AddRotation(const Vector3 delta, const bool isDegrees = false)
        {
            _eulerAngles += delta * (isDegrees ? FMath::DEGREES_TO_RAD : 1.f);
            _localToWorld.SetRotation(_eulerAngles, false);
            MarkDirty();
        }

        /**
         * Returns the rotation of the Transform in EulerAngles
         * @param inDegrees Output in degrees
         */
        Vector3 GetEulerAngles(const bool inDegrees = false) const
        {
            return _eulerAngles * (inDegrees ? FMath::RAD_TO_DEGREES : 1.f);
        }

        /**
         * Sets the scale of the Transform
         * @param scale World space scale
         */
        void SetScale(const Vector3 scale)
        {
            // Matrix4x4 cannot keep track of what component is negative only that is has a negative scale somewhere
            // by neutralizing the scale with our recorded scale per component scale can be functional
            _localToWorld.Scale(1.f/_scale);
            _scale = scale;
            MakeScaleSafe();
            _localToWorld.SetScale(_scale);
            MarkDirty();
        }

        /**
         * Scales the Transform
         * @param scale World space scale
         */
        void Scale(const Vector3 scale)
        {
            // Matrix4x4 cannot keep track of what component is negative only that is has a negative scale somewhere
            // by neutralizing the scale with our recorded scale per component scale can be functional
            _localToWorld.Scale(1.f/_scale);
            _scale *= scale;
            MakeScaleSafe();
            _localToWorld.SetScale(_scale);
            MarkDirty();
        }

        /**
         * Returns the scale of the Transform
         */
        Vector3 GetScale() const
        {
            return _scale;
        }

        /**
         * Returns the World to Local Matrix
         */
        Matrix4x4 GetWorldToLocal() const
        {
            return _localToWorld.GetInverse();
        }

        /**
         * Returns the Local to World Matrix
         */
        Matrix4x4 GetLocalToWorld() const
        {
            return _localToWorld;
        }

        /**
         * Returns the Transforms local X
         */
        Vector3 GetRight() const
        {
            return _localToWorld.GetRight();
        }

        /**
         * Returns the Transforms local Y
         */
        Vector3 GetUp() const
        {
            return _localToWorld.GetUp();
        }

        /**
         * Returns the Transforms local Z
         */
        Vector3 GetForward() const
        {
            return _localToWorld.GetForward();
        }

        void MarkDirty()
        {
            _isDirty = true;
        }

        void ClearDirty()
        {
            _isDirty = false;
        }

        bool IsDirty() const
        {
            return _isDirty;
        }

    private:
        void MakeScaleSafe();

        void SyncEulerAngles()
        {
            _eulerAngles = _localToWorld.GetEulerAngles();
        }

        Matrix4x4 _localToWorld;
        Vector3 _eulerAngles = 0;
        Vector3 _scale;
        bool _isDirty = false;
    };
}
