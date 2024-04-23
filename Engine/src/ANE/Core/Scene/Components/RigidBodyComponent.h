#pragma once
#include <reactphysics3d/body/RigidBody.h>

#include "Component.h"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Physics/Physics.h"
#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    class RigidBodyComponent : Component
    {
    public:
        ANE_COMPONENT_INIT(RigidBodyComponent)

        RigidBodyComponent(const Entity self) : Component(typeid(*this).name())
        {
            _rigidbody = GetPhysicsSystem().CreateRigidBody(self);
        }

        RigidBodyComponent(const Entity self, const BodyMode bodyType) : Component(typeid(*this).name())
        {
            _rigidbody = GetPhysicsSystem().CreateRigidBody(self);
            _rigidbody->setType(static_cast<reactphysics3d::BodyType>(bodyType));
        }

        rp3d::RigidBody* GetRigidBody() const
        {
            return _rigidbody;
        }

    private:

        rp3d::RigidBody* _rigidbody;
    };
}
