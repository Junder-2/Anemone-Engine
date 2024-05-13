#pragma once
#include "ANE/Core/Entity/ScriptableEntity.h"

namespace Engine
{
    struct RigidBodyComponent;

    class ColliderTest final : public ScriptableEntity
    {
    public:
        void OnCreate() override;
        void OnCollision(CollisionEventType type, const CollisionData& collisionData) override;
        void OnUpdate(float deltaTime) override;

    private:
        RigidBodyComponent* _rb {};
    };
}
