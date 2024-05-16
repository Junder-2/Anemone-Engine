#pragma once
#include "ANE/Math/Types/Vector3.h"
#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    struct Quaternion;

    class Collider
    {
    public:
        Collider(rp3d::Collider* collider, CollisionShapeType shapeType);
        virtual ~Collider() = default;

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
         * Sets the local space rotation of the collider
         */
        void SetScale(Vector3 scale);

        /**
         * Returns the local space rotation of the collider
         */
        Vector3 GetScale() const;

        /**
         * Sets the mask of what this collider should be able to collide with
         */
        void SetCollisionMask(CollisionLayerMask collisionMask) const;

        /**
         * Returns the mask of what this collider should be able to collide with
         */
        CollisionLayerMask GetCollisionMask() const;

        /**
         * Sets bits of what collision categories this collider is part of
         */
        void SetCollisionCategories(CollisionLayerMask collisionCategories) const;

        /**
         * Returns the bits of what collision categories this collider is part of
         */
        CollisionLayerMask GetCollisionCategories() const;

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
        void NotifyDirty() const;
        virtual void OnUpdateScale() {}

    protected:
        rp3d::Collider* _reactCollider;
        CollisionShapeType _shapeType;
        Vector3 _scale = 1.f;
    };
}
