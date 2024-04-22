#pragma once
#include <entt.hpp>

#include "ANE/Subsystem/SubSystem.h"
#include <reactphysics3d/reactphysics3d.h>

namespace Engine
{
    class Entity;
    class PhysicsLogger;
    struct TransformMatrix;

    class PhysicsSystem : public SubSystem
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem() override;
        void Free() override;

        reactphysics3d::PhysicsWorld& GetPhysicsWorld() const;

        reactphysics3d::RigidBody& CreateRigidBody(Entity entity);

    private:
        reactphysics3d::PhysicsCommon _physicsCommon {};
        reactphysics3d::PhysicsWorld* _world;

        PhysicsLogger* _physicsLogger;
    };
}
