#pragma once

#include "Component.h"
#include "ANE/Physics/Types/RigidBody.h"

namespace Engine
{
    enum class BodyType;
    class Entity;

    struct RigidBodyComponent : Component
    {
    public:
        ANE_COMPONENT_INIT(RigidBodyComponent)

        RigidBodyComponent(const Entity self);

        RigidBodyComponent(const Entity self, const BodyType bodyType);

        RigidBody* GetRigidBody() const { return _rigidbody; }

        static void RegisterComponentMetaData()
        {
            entt::meta<RigidBodyComponent>()
                .data<&RigidBodyComponent::_rigidbody>("RigidBody"_hs).prop("display_name"_hs, "RigidBody")
                .EDITABLE;
        }

    private:
        RigidBody* _rigidbody;
    };
}
