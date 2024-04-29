#include "anepch.h"
#include "SphereCollider.h"

#include "ANE/Math/FMath.h"
#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    SphereCollider::SphereCollider(rp3d::Collider* collider): Collider(collider, CollisionShapeType::Sphere) {}

    void SphereCollider::SetRadius(const float radius) const
    {
        WakeBody();
        reinterpret_cast<reactphysics3d::SphereShape*>(_reactCollider->getCollisionShape())->setRadius(FMath::Max(radius, MIN_SCALE));
    }

    float SphereCollider::GetRadius() const
    {
        return reinterpret_cast<reactphysics3d::SphereShape*>(_reactCollider->getCollisionShape())->getRadius();
    }
}
