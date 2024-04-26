#include "anepch.h"
#include "Collider.h"

#include "ANE/Math/Types/Quaternion.h"
#include "ANE/Math/Types/Vector3.h"

namespace Engine
{
    void Collider::SetTransform(const Vector3 position, const Quaternion& rotation) const
    {
        _reactCollider->setLocalToBodyTransform(rp3d::Transform(position, rotation));
    }

    void Collider::SetPosition(const Vector3 position) const
    {
        _reactCollider->setLocalToBodyTransform(rp3d::Transform(position, _reactCollider->getLocalToBodyTransform().getOrientation()));
    }

    Vector3 Collider::GetPosition() const
    {
        return Vector3::Convert(_reactCollider->getLocalToBodyTransform().getPosition());
    }

    void Collider::SetRotation(const Quaternion& rotation) const
    {
        _reactCollider->setLocalToBodyTransform(rp3d::Transform(_reactCollider->getLocalToBodyTransform().getPosition(), rotation));
    }

    Quaternion Collider::GetRotation() const
    {
        return Quaternion::Convert(_reactCollider->getLocalToBodyTransform().getOrientation());
    }

    void Collider::SetCollisionMask(const uint16_t collisionMask) const
    {
        _reactCollider->setCollideWithMaskBits(collisionMask);
    }

    uint16_t Collider::GetCollisionMask() const
    {
        return _reactCollider->getCollideWithMaskBits();
    }

    void Collider::SetCollisionCategories(const uint16_t collisionCategories) const
    {
        _reactCollider->setCollisionCategoryBits(collisionCategories);
    }

    uint16_t Collider::GetCollisionCategories() const
    {
        return _reactCollider->getCollisionCategoryBits();
    }

    void Collider::SetMaterial(const rp3d::Material& material) const
    {
        _reactCollider->setMaterial(material);
    }

    rp3d::Material& Collider::GetMaterial() const
    {
        return _reactCollider->getMaterial();
    }

    void Collider::SetTrigger(const bool isTrigger) const
    {
        _reactCollider->setIsTrigger(isTrigger);
    }

    bool Collider::IsTrigger() const
    {
        return _reactCollider->getIsTrigger();
    }

    CollisionShapeType Collider::GetShapeType() const
    {
        return _shapeType;
    }
}

