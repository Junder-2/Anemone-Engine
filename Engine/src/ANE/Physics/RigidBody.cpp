#include "anepch.h"
#include "RigidBody.h"

#include "ANE/Math/Types/Quaternion.h"
#include "Physics.h"

namespace Engine
{
    void RigidBody::Destroy()
    {
        GetPhysicsSystem().GetPhysicsWorld().destroyRigidBody(_reactRigidBody);
        _reactRigidBody = nullptr;
    }

    void RigidBody::SetTransform(const Vector3 position, const Quaternion& rotation) const
    {
        _reactRigidBody->setIsSleeping(false);
        _reactRigidBody->setTransform(rp3d::Transform(position, rotation));
    }

    void RigidBody::SetPosition(const Vector3 position) const
    {
        _reactRigidBody->setIsSleeping(false);
        _reactRigidBody->setTransform(rp3d::Transform(position, _reactRigidBody->getTransform().getOrientation()));
    }

    void RigidBody::SetRotation(const Quaternion& rotation) const
    {
        _reactRigidBody->setIsSleeping(false);
        _reactRigidBody->setTransform(rp3d::Transform(_reactRigidBody->getTransform().getPosition(), rotation));
    }

    void RigidBody::SetVelocity(const Vector3& newVelocity) const
    {
        _reactRigidBody->setLinearVelocity(newVelocity);
    }

    Vector3 RigidBody::GetVelocity() const
    {
        return Vector3::Convert(_reactRigidBody->getLinearVelocity());
    }

    void RigidBody::SetAngularVelocity(const Vector3& newAngularVelocity) const
    {
        _reactRigidBody->setAngularVelocity(newAngularVelocity);
    }

    Vector3 RigidBody::GetAngularVelocity() const
    {
        return Vector3::Convert(_reactRigidBody->getAngularVelocity());
    }

    void RigidBody::SetUseGravity(const bool enable) const
    {
        _reactRigidBody->enableGravity(enable);
    }

    bool RigidBody::IsGravityEnabled() const
    {
        return _reactRigidBody->isGravityEnabled();
    }

    void RigidBody::AddForce(const Vector3& force, const bool useMass) const
    {
        _reactRigidBody->applyWorldForceAtCenterOfMass(useMass ? force : force * _reactRigidBody->getMass());
    }

    void RigidBody::AddForceAtPoint(const Vector3& force, const Vector3& point, const bool useMass) const
    {
        _reactRigidBody->applyWorldForceAtWorldPosition(useMass ? force : force * _reactRigidBody->getMass(), point);
    }

    void RigidBody::AddLocalForce(const Vector3& force, const bool useMass) const
    {
        _reactRigidBody->applyLocalForceAtCenterOfMass(useMass ? force : force * _reactRigidBody->getMass());
    }

    void RigidBody::AddLocalForceAtPoint(const Vector3& force, const Vector3& point, const bool useMass) const
    {
        _reactRigidBody->applyLocalForceAtLocalPosition(useMass ? force : force * _reactRigidBody->getMass(), point);
    }

    void RigidBody::AddTorque(const Vector3& torque) const
    {
        _reactRigidBody->applyWorldTorque(torque);
    }

    void RigidBody::AddLocalTorque(const Vector3& torque) const
    {
        _reactRigidBody->applyLocalTorque(torque);
    }

    void RigidBody::SetMass(const float mass) const
    {
        _reactRigidBody->setMass(mass);
    }

    float RigidBody::GetMass() const
    {
        return _reactRigidBody->getMass();
    }

    void RigidBody::SetBodyType(BodyType type) const
    {
        _reactRigidBody->setType(static_cast<rp3d::BodyType>((int)type));
    }

    BodyType RigidBody::GetBodyType() const
    {
        return static_cast<BodyType>((int)_reactRigidBody->getType());
    }

    void RigidBody::SetActive(const bool enable) const
    {
        _reactRigidBody->setIsActive(enable);
    }

    bool RigidBody::IsActive() const
    {
        return _reactRigidBody->isActive();
    }

    void RigidBody::SetSleeping(const bool enable) const
    {
        _reactRigidBody->setIsSleeping(enable);
    }

    bool RigidBody::IsSleeping() const
    {
        return _reactRigidBody->isSleeping();
    }
}
