#pragma once
#include <entt.hpp>

#include "ANE/Subsystem/SubSystem.h"

namespace Engine
{
    struct TriggerData;
}

namespace Engine
{
    class CollisionListener;
}

namespace Engine
{
    struct CollisionData;
    enum class CollisionEventType;
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

        SphereCollider* CreateSphereCollider(rp3d::Entity reactEntity, float radius = 1.f);
        BoxCollider* CreateBoxCollider(rp3d::Entity reactEntity, const Vector3& halfExtents);
        CapsuleCollider* CreateCapsuleCollider(rp3d::Entity reactEntity, float radius = 1.f, float height = 2.f);

        void RemoveCollider(Entity entity, const Collider* collider);
        void RemoveCollider(rp3d::Entity reactEntity, const Collider* collider);

        rp3d::SphereShape* CreateSphereShape(float radius);
        rp3d::BoxShape* CreateBoxShape(const Vector3& halfExtents);
        rp3d::CapsuleShape* CreateCapsuleShape(float radius, float height);

        rp3d::Entity GetBodyEntity(Entity entity);
        Entity GetOwnerEntity(rp3d::Entity reactEntity);

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

    protected:
        void DispatchCollisionCallback(Entity entity, CollisionEventType type, const CollisionData& collisionData);
        void DispatchTriggerCallback(Entity entity, CollisionEventType type, const TriggerData& triggerData);

    private:
        rp3d::PhysicsCommon _physicsCommon {};
        rp3d::PhysicsWorld* _world;
        PhysicsLogger* _physicsLogger;
        CollisionListener* _collisionListener;

        entt::dense_map<rp3d::Entity, Entity> _reactEntity {};
        entt::dense_map<rp3d::Entity, RigidBody*> _reactRigidBody {};
        entt::dense_map<rp3d::Entity, Collider*> _reactCollider {};

        bool _hasAwokenBodies = false;

        friend class CollisionListener;
    };
}
