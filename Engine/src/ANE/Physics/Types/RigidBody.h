#pragma once
#include "reactphysics3d/reactphysics3d.h"

namespace Engine
{
    enum class BodyType;
    struct Vector3;
    struct Quaternion;

    /**
     * Wrapper for react rigidbody
     */
    class RigidBody
    {
    public:
        RigidBody(rp3d::RigidBody* rigidBody) : _reactRigidBody(rigidBody) {}

        /**
         * Sets the transform of the rigidbody
         */
        void SetTransform(Vector3 position, const Quaternion& rotation) const;

        /**
         * Sets the position of the rigidbody
         */
        void SetPosition(Vector3 position) const;

        /**
         * Returns the position of the rigidbody
         */
        Vector3 GetPosition() const;

        /**
         * Sets the rotation of the rigidbody
         */
        void SetRotation(const Quaternion& rotation) const;

        /**
         * Returns the rotation of the rigidbody
         */
        Quaternion GetRotation() const;

        /**
         * Sets the linear velocity of the rigidbody
         */
        void SetVelocity(const Vector3& newVelocity) const;

        /**
         * Returns the linear velocity of the rigidbody
         */
        Vector3 GetVelocity() const;

        /**
         * Sets the angular velocity of the rigidbody
         */
        void SetAngularVelocity(const Vector3& newAngularVelocity) const;

        /**
         * Returns the angular velocity of the rigidbody
         */
        Vector3 GetAngularVelocity() const;

        /**
         * Sets if rigidbody should use gravity
         */
        void SetUseGravity(bool enable) const;

        /**
         * Returns if gravity is enabled for the rigidbody
         */
        bool IsGravityEnabled() const;

        /**
         * Adds a force in world space from the rigidbody's center
         */
        void AddForce(const Vector3& force, bool useMass = true) const;

        /**
         * Adds a force in world space from the points location
         */
        void AddForceAtPoint(const Vector3& force, const Vector3& point, bool useMass = true) const;

        /**
         * Adds a force in local space from the rigidbody's center
         */
        void AddLocalForce(const Vector3& force, bool useMass = true) const;

        /**
         * Adds a force in local space from the points location
         */
        void AddLocalForceAtPoint(const Vector3& force, const Vector3& point, bool useMass = true) const;

        /**
         * Adds a torque in world space to the rigidbody
         */
        void AddTorque(const Vector3& torque) const;

        /**
         * Adds a torque in local space to the rigidbody
         */
        void AddLocalTorque(const Vector3& torque) const;

        /**
         * Sets the mass of the rigidbody
         */
        void SetMass(float mass) const;

        /**
         * Returns the mass of the rigidbody
         */
        float GetMass() const;

        /**
         * Sets the BodyMode of the rigidbody
         */
        void SetBodyType(BodyType type) const;

        /**
         * Returns the BodyMode of the rigidbody
         */
        BodyType GetBodyType() const;

        /**
         * Sets the active state of the rigidbody
         */
        void SetActive(bool enable) const;

        /**
         * Returns the active state of the rigidbody
         */
        bool IsActive() const;

        /**
         * Sets the sleeping state of the rigidbody
         */
        void SetSleeping(bool enable) const;

        /**
         * Returns the sleeping state of the rigidbody
         */
        bool IsSleeping() const;

        /**
         * Destroys the rigidbody and any connected joints
         */
        void Destroy();

        rp3d::RigidBody& GetReactRigidBody() const
        {
            return *_reactRigidBody;
        }

    private:
        rp3d::RigidBody* _reactRigidBody;
    };
}
