#include "anepch.h"
#include "CapsuleCollider.h"

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
        dynamic_cast<reactphysics3d::CapsuleShape*>(_reactCollider->getCollisionShape())->setRadius(radius);
    }

    void CapsuleCollider::SetHeight(const float height) const
    {
        WakeBody();
        dynamic_cast<reactphysics3d::CapsuleShape*>(_reactCollider->getCollisionShape())->setHeight(height);
    }

    float CapsuleCollider::GetRadius() const
    {
        return dynamic_cast<reactphysics3d::CapsuleShape*>(_reactCollider->getCollisionShape())->getRadius();
    }

    float CapsuleCollider::GetHeight() const
    {
        return dynamic_cast<reactphysics3d::CapsuleShape*>(_reactCollider->getCollisionShape())->getHeight();
    }
}
