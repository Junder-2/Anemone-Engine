#include "anepch.h"
#include "SphereCollider.h"

#include "ANE/Math/FMath.h"
#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    SphereCollider::SphereCollider(rp3d::Collider* collider): Collider(collider, CollisionShapeType::Sphere)
    {
        const auto sphereCollider = reinterpret_cast<reactphysics3d::SphereShape*>(_reactCollider->getCollisionShape());
        _radius = sphereCollider->getRadius();
    }

    void SphereCollider::SetRadius(const float radius)
    {
        _radius = FMath::Max(radius, MIN_PHYS);

        NotifyDirty();
        const auto sphereCollider = reinterpret_cast<reactphysics3d::SphereShape*>(_reactCollider->getCollisionShape());
        const float scale = FMath::Max(FMath::Max(_scale.X, _scale.Y), _scale.Z);
        sphereCollider->setRadius(FMath::Max(_radius*scale, MIN_PHYS));
    }

    float SphereCollider::GetRadius() const
    {
        return _radius;
    }

    void SphereCollider::OnUpdateScale()
    {
        SetRadius(GetRadius());
    }
}
