#pragma once

namespace Engine
{
    enum class CollisionShapeType;
    struct Quaternion;
    struct Vector3;

    class Collider
    {
    public:
        Collider(rp3d::Collider* collider, const CollisionShapeType shapeType) : _reactCollider(collider), _shapeType(shapeType) {}

        /**
         * Sets the local space transform of the collider
         */
        void SetTransform(Vector3 position, const Quaternion& rotation) const;

        /**
         * Sets the local space position of the collider
         */
        void SetPosition(Vector3 position) const;

        /**
         * Returns the local space position of the collider
         */
        Vector3 GetPosition() const;

        /**
         * Sets the local space rotation of the collider
         */
        void SetRotation(const Quaternion& rotation) const;

        /**
         * Returns the local space rotation of the collider
         */
        Quaternion GetRotation() const;

        /**
         * Sets the local space rotation of the collider
         */
        void SetRotation(const Vector3& rotation, bool inDegrees = false) const;

        /**
         * Returns the local space rotation of the collider
         */
        Vector3 GetEulerAngles(bool inDegrees = false) const;

        /**
         * Sets the mask of what this collider should be able to collide with
         */
        void SetCollisionMask(uint16_t collisionMask) const;

        /**
         * Returns the mask of what this collider should be able to collide with
         */
        uint16_t GetCollisionMask() const;

        /**
         * Sets bits of what collision categories this collider is part of
         */
        void SetCollisionCategories(uint16_t collisionCategories) const;

        /**
         * Returns the bits of what collision categories this collider is part of
         */
        uint16_t GetCollisionCategories() const;

        /**
         * Sets the physics material
         */
        void SetMaterial(const rp3d::Material& material) const;

        /**
         * Returns the physics material
         */
        rp3d::Material& GetMaterial() const;

        /**
         * Sets if this collider is trigger
         */
        void SetTrigger(bool isTrigger) const;

        /**
         * Returns if this collider is trigger
         */
        bool IsTrigger() const;

        CollisionShapeType GetShapeType() const;

        rp3d::Collider& GetReactCollider() const
        {
            return *_reactCollider;
        }

    protected:
        void WakeBody() const;

    protected:
        rp3d::Collider* _reactCollider;
        CollisionShapeType _shapeType;
    };
}
