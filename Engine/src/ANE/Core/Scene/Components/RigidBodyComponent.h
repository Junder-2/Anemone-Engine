#pragma once
#include <reactphysics3d/body/RigidBody.h>

#include "Component.h"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Physics/Physics.h"

namespace Engine
{
    class RigidBodyComponent : Component
    {
    public:
        ANE_COMPONENT_INIT(RigidBodyComponent)

        RigidBodyComponent(const Entity self) : Component(typeid(*this).name())
        {
            Rigidbody = &GetPhysicsSystem().CreateRigidBody(self);
        }

        reactphysics3d::RigidBody* Rigidbody;
    };
}
