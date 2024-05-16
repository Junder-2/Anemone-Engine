#include "anepch.h"
#include "RigidBodyComponent.h"

#include "ANE/Core/Entity/Entity.h"
#include "ANE/Physics/Physics.h"
#include "ANE/Physics/Types/RigidBody.h"

namespace Engine
{
    RigidBodyComponent::RigidBodyComponent(const Entity self): Component(typeid(*this).name())
    {
        _rigidbody = GetPhysicsSystem().CreateRigidBody(self);
        _rigidbody->SetBodyType(BodyType::Static);
    }

    RigidBodyComponent::RigidBodyComponent(const Entity self, const BodyType bodyType): Component(typeid(*this).name())
    {
        _rigidbody = GetPhysicsSystem().CreateRigidBody(self);
        _rigidbody->SetBodyType(bodyType);
    }
}
