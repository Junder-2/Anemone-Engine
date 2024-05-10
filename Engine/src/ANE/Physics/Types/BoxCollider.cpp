#include "anepch.h"
#include "BoxCollider.h"

#include "ANE/Math/VMath.h"
#include "ANE/Math/Types/Vector3.h"
#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    BoxCollider::BoxCollider(rp3d::Collider* collider) : Collider(collider, CollisionShapeType::Box)
    {
        const auto boxCollider = reinterpret_cast<reactphysics3d::BoxShape*>(_reactCollider->getCollisionShape());
        _halfSize = Vector3::Convert(boxCollider->getHalfExtents());
    }

    void BoxCollider::SetHalfSize(const Vector3 halfSize)
    {
        _halfSize = Math::Max(halfSize, MIN_SCALE);

        WakeBody();
        const auto boxCollider = reinterpret_cast<reactphysics3d::BoxShape*>(_reactCollider->getCollisionShape());
        boxCollider->setHalfExtents(Math::Max(_halfSize*_scale, MIN_SCALE));

        ForceUpdateBody();
    }

    Vector3 BoxCollider::GetHalfSize() const
    {
        return _halfSize;
    }

    void BoxCollider::OnUpdateScale()
    {
        SetHalfSize(GetHalfSize());
    }
}
