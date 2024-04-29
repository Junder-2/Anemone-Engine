#include "anepch.h"
#include "BoxCollider.h"

#include "ANE/Math/VMath.h"
#include "ANE/Math/Types/Vector3.h"
#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    BoxCollider::BoxCollider(rp3d::Collider* collider) : Collider(collider, CollisionShapeType::Box) {}

    void BoxCollider::SetHalfSize(const Vector3 halfSize) const
    {
        WakeBody();
        reinterpret_cast<reactphysics3d::BoxShape*>(_reactCollider->getCollisionShape())->setHalfExtents(Math::Max(halfSize, Vector3::OneVector()*MIN_SCALE));
    }

    Vector3 BoxCollider::GetHalfSize() const
    {
        return Vector3::Convert(reinterpret_cast<reactphysics3d::BoxShape*>(_reactCollider->getCollisionShape())->getHalfExtents());
    }
}
