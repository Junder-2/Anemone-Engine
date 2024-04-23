#pragma once

#include "Component.h"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Physics/Physics.h"
#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    struct ColliderComponent : Component
    {
    public:
        ANE_COMPONENT_INIT(ColliderComponent)

        ColliderComponent(const Entity self, const Vector3 halfExtents) : Component(typeid(*this).name())
        {
            _collider = GetPhysicsSystem().CreateBoxCollider(self, halfExtents);
            _shapeType = CollisionShapeType::Box;
        }

        ColliderComponent(const Entity self, const float radius) : Component(typeid(*this).name())
        {
            _collider = GetPhysicsSystem().CreateSphereCollider(self, radius);
            _shapeType = CollisionShapeType::Sphere;
        }

        ColliderComponent(const Entity self, const float radius, const float height) : Component(typeid(*this).name())
        {
            _collider = GetPhysicsSystem().CreateCapsuleCollider(self, radius, height);
            _shapeType = CollisionShapeType::Capsule;
        }

        CollisionShapeType GetShapeType() const
        {
            return _shapeType;
        }

        rp3d::Collider* GetCollider() const
        {
            return _collider;
        }

    private:
        CollisionShapeType _shapeType;
        rp3d::Collider* _collider;
    };
}
