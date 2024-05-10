#pragma once
#include "ANE/Subsystem/SubSystem.h"

namespace Engine
{
    enum class PhysicsDebugDisplayFlag;
    class Scene;
    class Collider;
    class CapsuleCollider;
    class BoxCollider;
    class SphereCollider;
    class RigidBody;
    class Entity;
    class PhysicsLogger;
    struct Vector3;
    struct TransformMatrix;

    class PhysicsSystem : public SubSystem
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem() override;
        void Free() override;

        rp3d::PhysicsWorld& GetPhysicsWorld() const;
        RigidBody* CreateRigidBody(Entity entity);
        SphereCollider* CreateSphereCollider(Entity entity, float radius = 1.f);
        BoxCollider* CreateBoxCollider(Entity entity, const Vector3& halfExtents);
        CapsuleCollider* CreateCapsuleCollider(Entity entity, float radius = 1.f, float height = 2.f);

        void RemoveCollider(Entity entity, const Collider* collider);

        rp3d::SphereShape* CreateSphereShape(float radius);
        rp3d::BoxShape* CreateBoxShape(const Vector3& halfExtents);
        rp3d::CapsuleShape* CreateCapsuleShape(float radius, float height);

        void WakeBodies();

        void PhysicsUpdate(float timeStep, Scene* scene);
        void UpdateRigidBodies(float factor, Scene* scene);

        #ifndef ANE_DIST
        void EnableDebugRendering(bool enable);
        bool IsDebugRendering() const { return _isDebugRendering; }

        void EnableDebugFlag(PhysicsDebugDisplayFlag displayFlag, bool enable) const;
        bool IsDebugDisplayFlag(PhysicsDebugDisplayFlag displayFlag) const;

        void SetDebugDisplayAlpha(const float value) { _debugDisplayAlpha = value;}
        float GetDebugDisplayAlpha() const { return _debugDisplayAlpha; }
        #endif

    private:
        #ifndef ANE_DIST
        void DebugDraw();
        rp3d::DebugRenderer* _debugRenderer;
        bool _isDebugRendering;
        float _debugDisplayAlpha;
        #endif

    private:
        rp3d::PhysicsCommon _physicsCommon {};
        rp3d::PhysicsWorld* _world;
        PhysicsLogger* _physicsLogger;

        bool _hasAwokenBodies = false;

    };
}
