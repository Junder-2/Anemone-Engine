#pragma once

#include "ANE/Subsystem/SubSystem.h"
#include <reactphysics3d/reactphysics3d.h>

namespace Engine
{
    struct Vector3;
    class Entity;
    class PhysicsLogger;
    struct TransformMatrix;

    class PhysicsSystem : public SubSystem
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem() override;
        void Free() override;

        rp3d::PhysicsWorld& GetPhysicsWorld() const;
        rp3d::RigidBody* CreateRigidBody(Entity entity);
        rp3d::Collider* CreateSphereCollider(Entity entity, float radius = 1.f);
        rp3d::Collider* CreateBoxCollider(Entity entity, const Vector3& halfExtents);
        rp3d::Collider* CreateCapsuleCollider(Entity entity, float radius = 1.f, float height = 2.f);

        rp3d::SphereShape* CreateSphereShape(float radius);
        rp3d::BoxShape* CreateBoxShape(const Vector3& halfExtents);
        rp3d::CapsuleShape* CreateCapsuleShape(float radius, float height);

    private:
        rp3d::PhysicsCommon _physicsCommon {};
        rp3d::PhysicsWorld* _world;
        rp3d::DebugRenderer* _debugRenderer;

        PhysicsLogger* _physicsLogger;
    };
}
