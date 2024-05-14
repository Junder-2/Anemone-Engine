#include "anepch.h"
#include "Collider.h"

#include "ANE/Math/VMath.h"
#include "ANE/Math/Types/Quaternion.h"
#include "ANE/Math/Types/Vector3.h"
#include "ANE/Physics/Physics.h"
#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    Collider::Collider(rp3d::Collider* collider, const CollisionShapeType shapeType): _reactCollider(collider), _shapeType(shapeType)
    {
        SetCollisionMask(EnumCast(CollisionLayerPreset::Default));
    }

    void Collider::SetTransform(const Vector3 position, const Quaternion& rotation) const
    {
        WakeBody();
        _reactCollider->setLocalToBodyTransform(rp3d::Transform(position, rotation));
    }

    void Collider::SetPosition(const Vector3 position) const
    {
        SetTransform(position, Quaternion::Convert(_reactCollider->getLocalToBodyTransform().getOrientation()));
    }

    Vector3 Collider::GetPosition() const
    {
        return Vector3::Convert(_reactCollider->getLocalToBodyTransform().getPosition());
    }

    void Collider::SetRotation(const Quaternion& rotation) const
    {
        SetTransform(Vector3::Convert(_reactCollider->getLocalToBodyTransform().getPosition()), rotation);
    }

    Quaternion Collider::GetRotation() const
    {
        return Quaternion::Convert(_reactCollider->getLocalToBodyTransform().getOrientation());
    }

    void Collider::SetRotation(const Vector3& rotation, const bool inDegrees /*= false*/) const
    {
        SetTransform(Vector3::Convert(_reactCollider->getLocalToBodyTransform().getPosition()), Quaternion::FromEulerAngles(rotation, inDegrees));
    }

    Vector3 Collider::GetEulerAngles(const bool inDegrees /*= false*/) const
    {
        return Quaternion::Convert(_reactCollider->getLocalToBodyTransform().getOrientation()).GetEulerAngles(inDegrees);
    }

    void Collider::SetScale(const Vector3 scale)
    {
        _scale = Math::Max(Math::Abs(scale), MIN_SCALE);
        OnUpdateScale();
    }

    Vector3 Collider::GetScale() const
    {
        return _scale;
    }

    void Collider::SetCollisionMask(const CollisionLayerMask collisionMask) const
    {
        _reactCollider->setCollideWithMaskBits(collisionMask);
    }

    CollisionLayerMask Collider::GetCollisionMask() const
    {
        return _reactCollider->getCollideWithMaskBits();
    }

    void Collider::SetCollisionCategories(const CollisionLayerMask collisionCategories) const
    {
        _reactCollider->setCollisionCategoryBits(collisionCategories);
    }

    CollisionLayerMask Collider::GetCollisionCategories() const
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

    void Collider::ForceUpdateBody() const
    {
        SetPosition(GetPosition());
    }

    void Collider::WakeBody() const
    {
        GetPhysicsSystem().WakeBodies();
    }
}

