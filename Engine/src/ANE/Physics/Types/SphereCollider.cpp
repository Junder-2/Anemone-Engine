#include "anepch.h"
#include "SphereCollider.h"

#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    SphereCollider::SphereCollider(rp3d::Collider* collider): Collider(collider, CollisionShapeType::Sphere) {}

    void SphereCollider::SetRadius(const float radius) const
    {
        dynamic_cast<reactphysics3d::SphereShape*>(_reactCollider->getCollisionShape())->setRadius(radius);
    }

    float SphereCollider::GetRadius() const
    {
        return dynamic_cast<reactphysics3d::SphereShape*>(_reactCollider->getCollisionShape())->getRadius();
    }
}
