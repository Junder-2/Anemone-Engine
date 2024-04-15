#pragma once
#include "ANE/Subsystem/SubSystem.h"
#include <reactphysics3d/reactphysics3d.h>

namespace Engine
{
    using namespace reactphysics3d;
    class PhysicsLogger;
    struct TransformMatrix;

    class PhysicsSystem : public SubSystem
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem() override;

        RigidBody& CreateRigidBody(const TransformMatrix& transformMatrix);

    private:
        PhysicsCommon _physicsCommon {};
        PhysicsWorld* _world;

        PhysicsLogger* _physicsLogger;
    };
}
