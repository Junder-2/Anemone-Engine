﻿#include "anepch.h"
#include "RigidBody.h"

#include "ANE/Math/FMath.h"
#include "ANE/Math/Types/Quaternion.h"
#include "ANE/Physics/Physics.h"
#include "ANE/Utilities/API.h"

namespace Engine
{
    void RigidBody::Destroy()
    {
        GetPhysicsSystem().GetPhysicsWorld().destroyRigidBody(_reactRigidBody);
        _reactRigidBody = nullptr;
    }

    void RigidBody::TryUpdate() const
    {
        if(IsAutoMass()) UpdateMass();
        if(IsAutoCenterOfMass()) UpdateCenterOfMass();
    }

    void RigidBody::SetTransform(const Vector3 position, const Quaternion& rotation, const bool teleport /*= false*/)
    {
        MarkDirty();
        _reactRigidBody->setTransform(rp3d::Transform(position, rotation));
        if(teleport)
        {
            SetVelocity(0);
            SetAngularVelocity(0);
        }
    }

    void RigidBody::SetPosition(const Vector3 position, const bool teleport /*= false*/)
    {
        SetTransform(position, GetRotation(), teleport);
    }

    Vector3 RigidBody::GetPosition() const
    {
        return Vector3::Convert(_reactRigidBody->getTransform().getPosition());
    }

    void RigidBody::SetRotation(const Quaternion& rotation, const bool teleport /*= false*/)
    {
        SetTransform(GetPosition(), rotation, teleport);
    }

    Quaternion RigidBody::GetRotation() const
    {
        return Quaternion::Convert(_reactRigidBody->getTransform().getOrientation());
    }

    void RigidBody::SetVelocity(const Vector3& newVelocity) const
    {
        _reactRigidBody->setLinearVelocity(newVelocity);
    }

    void RigidBody::AddVelocity(const Vector3& force) const
    {
        SetVelocity(GetVelocity() + force);
    }

    Vector3 RigidBody::GetVelocity() const
    {
        return Vector3::Convert(_reactRigidBody->getLinearVelocity());
    }

    void RigidBody::SetAngularVelocity(const Vector3& newAngularVelocity) const
    {
        _reactRigidBody->setAngularVelocity(newAngularVelocity);
    }

    void RigidBody::AddAngularVelocity(const Vector3& force) const
    {
        SetAngularVelocity(GetAngularVelocity() + force);
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

    void RigidBody::AddForce(const Vector3& force, const bool useMass /*= true*/) const
    {
        Vector3 finalForce = force / API::PHYSICS_TIMESTEP;
        if(!useMass) finalForce *= _reactRigidBody->getMass();
        _reactRigidBody->applyWorldForceAtCenterOfMass(finalForce);
    }

    void RigidBody::AddForceAtPoint(const Vector3& force, const Vector3& point, const bool useMass /*= true*/) const
    {
        Vector3 finalForce = force / API::PHYSICS_TIMESTEP;
        if(!useMass) finalForce *= _reactRigidBody->getMass();
        _reactRigidBody->applyWorldForceAtWorldPosition(finalForce, point);
    }

    void RigidBody::AddLocalForce(const Vector3& force, const bool useMass /*= true*/) const
    {
        Vector3 finalForce = force / API::PHYSICS_TIMESTEP;
        if(!useMass) finalForce *= _reactRigidBody->getMass();
        _reactRigidBody->applyLocalForceAtCenterOfMass(finalForce);
    }

    void RigidBody::AddLocalForceAtPoint(const Vector3& force, const Vector3& point, const bool useMass /*= true*/) const
    {
        Vector3 finalForce = force / API::PHYSICS_TIMESTEP;
        if(!useMass) finalForce *= _reactRigidBody->getMass();
        _reactRigidBody->applyLocalForceAtLocalPosition(finalForce, point);
    }

    void RigidBody::AddTorque(const Vector3& torque) const
    {
        const Vector3 finalTorque = torque / API::PHYSICS_TIMESTEP;
        _reactRigidBody->applyWorldTorque(finalTorque);
    }

    void RigidBody::AddLocalTorque(const Vector3& torque) const
    {
        const Vector3 finalTorque = torque / API::PHYSICS_TIMESTEP;
        _reactRigidBody->applyLocalTorque(finalTorque);
    }

    void RigidBody::SetDamping(const float newDamping) const
    {
        _reactRigidBody->setLinearDamping(FMath::Max0(newDamping));
    }

    float RigidBody::GetDamping() const
    {
        return _reactRigidBody->getLinearDamping();
    }

    void RigidBody::SetAngularDamping(const float newDamping) const
    {
        _reactRigidBody->setAngularDamping(FMath::Max0(newDamping));
    }

    float RigidBody::GetAngularDamping() const
    {
        return _reactRigidBody->getAngularDamping();
    }

    void RigidBody::UpdateCenterOfMass() const
    {
        _reactRigidBody->updateLocalCenterOfMassFromColliders();
    }

    void RigidBody::SetLocalCenterOfMass(const Vector3& centerOfMass) const
    {
        _reactRigidBody->setLocalCenterOfMass(centerOfMass);
    }

    Vector3 RigidBody::GetLocalCenterOfMass() const
    {
        return Vector3::Convert(_reactRigidBody->getLocalCenterOfMass());
    }

    void RigidBody::UpdateMass() const
    {
        _reactRigidBody->updateMassFromColliders();
    }

    void RigidBody::SetMass(const float mass) const
    {
        _reactRigidBody->setMass(FMath::Max(mass, MIN_PHYS));
    }

    float RigidBody::GetMass() const
    {
        return _reactRigidBody->getMass();
    }

    void RigidBody::SetBodyType(BodyType type)
    {
        if(type == GetBodyType()) return;
        MarkDirty();
        _bodyType = type;
        _reactRigidBody->setType(static_cast<rp3d::BodyType>(type));
    }

    BodyType RigidBody::GetBodyType() const
    {
        return static_cast<BodyType>(_reactRigidBody->getType());
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

    void RigidBody::SetAutoMass(const bool enable)
    {
        if(enable == IsAutoMass()) return;

        _flags ^= EnumCast(RigidBodyFlag::AutoMass);

        MarkDirty();
    }

    void RigidBody::SetAutoCenterOfMass(const bool enable)
    {
        if(enable == IsAutoCenterOfMass()) return;

        _flags ^= EnumCast(RigidBodyFlag::AutoCenterOfMass);

        MarkDirty();
    }

    void RigidBody::MarkDirty()
    {
        if(IsDirty()) return;

        _reactRigidBody->setType(rp3d::BodyType::KINEMATIC);
        _flags |= EnumCast(RigidBodyFlag::IsDirty);
    }

    void RigidBody::ClearDirty()
    {
        _reactRigidBody->setType(static_cast<rp3d::BodyType>(_bodyType));
        _flags &= ~EnumCast(RigidBodyFlag::IsDirty);
    }
}
