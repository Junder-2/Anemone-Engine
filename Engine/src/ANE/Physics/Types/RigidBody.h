#pragma once

namespace Engine
{
    enum class BodyType;
    struct Vector3;
    struct Quaternion;

    typedef uint8 RigidBodyFlags;
    enum class RigidBodyFlag : uint8
    {
        None = 0,
        AutoMass = BIT(0),
        AutoCenterOfMass = BIT(1),
        IsDirty = BIT(2),
    };
    ENUM_CLASS_OPERATORS(RigidBodyFlag)

    /**
     * Wrapper for react rigidbody
     */
    class RigidBody
    {
    public:
        RigidBody(rp3d::RigidBody* rigidBody) : _reactRigidBody(rigidBody), _flags(0)
        {
            SetAutoMass(true);
            SetAutoCenterOfMass(true);
            SetActive(true);
            _reactRigidBody->setIsActive(true);
            ClearDirty();
        }

        /**
         * Sets the transform of the rigidbody
         */
        void SetTransform(Vector3 position, const Quaternion& rotation, bool teleport = false);

        /**
         * Sets the position of the rigidbody
         */
        void SetPosition(Vector3 position, bool teleport = false);

        /**
         * Returns the position of the rigidbody
         */
        Vector3 GetPosition() const;

        /**
         * Sets the rotation of the rigidbody
         */
        void SetRotation(const Quaternion& rotation, bool teleport = false);

        /**
         * Returns the rotation of the rigidbody
         */
        Quaternion GetRotation() const;

        /**
         * Sets the linear velocity of the rigidbody
         */
        void SetVelocity(const Vector3& newVelocity) const;

        /**
         * Adds an instant force to the rigidbody
         */
        void AddVelocity(const Vector3& force) const;

        /**
         * Returns the linear velocity of the rigidbody
         */
        Vector3 GetVelocity() const;

        /**
         * Sets the angular velocity of the rigidbody
         */
        void SetAngularVelocity(const Vector3& newAngularVelocity) const;

        /**
         * Adds an instant force to the rigidbody
         */
        void AddAngularVelocity(const Vector3& force) const;

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
         * Adds a force (newtons) in world space from the rigidbody's center
         */
        void AddForce(const Vector3& force, bool useMass = true) const;

        /**
         * Adds a force (newtons) in world space from the points location
         */
        void AddForceAtPoint(const Vector3& force, const Vector3& point, bool useMass = true) const;

        /**
         * Adds a force (newtons) in local space from the rigidbody's center
         */
        void AddLocalForce(const Vector3& force, bool useMass = true) const;

        /**
         * Adds a force (newtons) in local space from the points location
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
         * Sets the damping factor of velocity
         */
        void SetDamping(float newDamping) const;

        /**
         * Returns the damping factor of velocity
         */
        float GetDamping() const;

        /**
         * Sets the damping factor of angular velocity
         */
        void SetAngularDamping(float newDamping) const;

        /**
         * Returns the damping factor of angular velocity
         */
        float GetAngularDamping() const;

        /**
         * Computes the center of mass from the colliders
         */
        void UpdateCenterOfMass() const;

        /**
         * Set the center of mass of the body in localspace
         */
        void SetLocalCenterOfMass(const Vector3& centerOfMass) const;

        /**
         * Return the center of mass of the body in localspace
         */
        Vector3 GetLocalCenterOfMass() const;

        /**
         * Computes the center of mass from the colliders
         */
        void UpdateMass() const;

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
        void SetBodyType(BodyType type);

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
         * Sets if mass should be automatically computed from colliders
         */
        void SetAutoMass(bool enable);

        /**
         * Sets if mass should be automatically computed from colliders
         */
        bool IsAutoMass() const { return _flags & EnumCast(RigidBodyFlag::AutoMass); }

        /**
         * Sets if center of mass should be automatically computed from colliders
         */
        void SetAutoCenterOfMass(bool enable);

        /**
         * Sets if center of mass should be automatically computed from colliders
         */
        bool IsAutoCenterOfMass() const { return _flags & EnumCast(RigidBodyFlag::AutoCenterOfMass); }

        bool IsDirty() const { return _flags & EnumCast(RigidBodyFlag::IsDirty); }

        /**
         * Destroys the rigidbody and any connected joints
         */
        void Destroy();

        rp3d::RigidBody& GetReactRigidBody() const
        {
            return *_reactRigidBody;
        }

    private:
        void MarkDirty();
        void ClearDirty();

        void TryUpdate() const;

    private:
        rp3d::RigidBody* _reactRigidBody;
        RigidBodyFlags _flags;
        BodyType _bodyType;

        friend class PhysicsSystem;
        friend class Collider;
    };
}
