#pragma once
#include "Component.h"

namespace Engine
{
    enum class BodyType;
    class RigidBody;
    class Entity;

    struct RigidBodyComponent : Component
    {
        ANE_COMPONENT_INIT(RigidBodyComponent)

        RigidBodyComponent(Entity self);

        RigidBodyComponent(Entity self, BodyType bodyType);

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
