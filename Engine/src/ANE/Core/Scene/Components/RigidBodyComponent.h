#pragma once

#include "Component.h"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Physics/Physics.h"
#include "ANE/Physics/PhysicsTypes.h"
#include "ANE/Physics/RigidBody.h"

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

        RigidBodyComponent(const Entity self, const BodyType bodyType) : Component(typeid(*this).name())
        {
            _rigidbody = GetPhysicsSystem().CreateRigidBody(self);
            _rigidbody->SetBodyType(bodyType);
        }

        RigidBody* GetRigidBody() const
        {
            return _rigidbody;
        }

    private:

        RigidBody* _rigidbody;
    };
}
