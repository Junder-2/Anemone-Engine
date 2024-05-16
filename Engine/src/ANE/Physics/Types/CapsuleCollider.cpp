#include "anepch.h"
#include "CapsuleCollider.h"

#include "ANE/Math/FMath.h"
#include "ANE/Math/Types/Vector3.h"
#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    CapsuleCollider::CapsuleCollider(rp3d::Collider* collider) : Collider(collider, CollisionShapeType::Capsule)
    {
        const auto capsuleCollider = reinterpret_cast<reactphysics3d::CapsuleShape*>(_reactCollider->getCollisionShape());
        _radius = capsuleCollider->getRadius();
        _height = capsuleCollider->getHeight();
    }

    void CapsuleCollider::SetSize(const float radius, const float height)
    {
        SetRadius(radius);
        SetHeight(height);
    }

    void CapsuleCollider::SetRadius(const float radius)
    {
        _radius = FMath::Max(radius, MIN_PHYS);

        NotifyDirty();
        const auto capsuleCollider = reinterpret_cast<reactphysics3d::CapsuleShape*>(_reactCollider->getCollisionShape());
        const float scale = FMath::Max(_scale.X, _scale.Z);
        capsuleCollider->setRadius(FMath::Max(_radius*scale, MIN_PHYS));
    }

    void CapsuleCollider::SetHeight(const float height)
    {
        _height = FMath::Max(height, MIN_PHYS);

        NotifyDirty();
        const auto capsuleCollider = reinterpret_cast<reactphysics3d::CapsuleShape*>(_reactCollider->getCollisionShape());
        capsuleCollider->setHeight(FMath::Max(_height*_scale.Y, MIN_PHYS));
    }

    float CapsuleCollider::GetRadius() const
    {
        return _radius;
    }

    float CapsuleCollider::GetHeight() const
    {
        return _height;
    }

    void CapsuleCollider::OnUpdateScale()
    {
        SetSize(GetRadius(), GetHeight());
    }
}
