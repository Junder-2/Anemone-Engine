#include "anepch.h"
#include "CapsuleCollider.h"

#include "ANE/Math/FMath.h"
#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    CapsuleCollider::CapsuleCollider(rp3d::Collider* collider) : Collider(collider, CollisionShapeType::Capsule) {}

    void CapsuleCollider::SetSize(const float radius, float height) const
    {
        SetRadius(radius);
        SetHeight(height);
    }

    void CapsuleCollider::SetRadius(const float radius) const
    {
        WakeBody();
        reinterpret_cast<reactphysics3d::CapsuleShape*>(_reactCollider->getCollisionShape())->setRadius(FMath::Max(radius, MIN_SCALE));
    }

    void CapsuleCollider::SetHeight(const float height) const
    {
        WakeBody();
        reinterpret_cast<reactphysics3d::CapsuleShape*>(_reactCollider->getCollisionShape())->setHeight(FMath::Max(height, MIN_SCALE));
    }

    float CapsuleCollider::GetRadius() const
    {
        return reinterpret_cast<reactphysics3d::CapsuleShape*>(_reactCollider->getCollisionShape())->getRadius();
    }

    float CapsuleCollider::GetHeight() const
    {
        return reinterpret_cast<reactphysics3d::CapsuleShape*>(_reactCollider->getCollisionShape())->getHeight();
    }
}
