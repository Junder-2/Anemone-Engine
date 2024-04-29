#include "anepch.h"
#include "PhysicsSystem.h"

#include "PhysicsLogger.h"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Math/Types/TransformMatrix.h"
#include "ANE/Core/Scene/Components/RigidBodyComponent.h"
#include "Types/BoxCollider.h"
#include "Types/CapsuleCollider.h"
#include "Types/RigidBody.h"
#include "Types/SphereCollider.h"

namespace Engine
{
    PhysicsSystem::PhysicsSystem()
    {
        _physicsLogger = new PhysicsLogger();
        _physicsCommon.setLogger(_physicsLogger);

        rp3d::PhysicsWorld::WorldSettings worldSettings;
        worldSettings.worldName = "PhysicsWorld";
        //worldSettings.isSleepingEnabled = false; // Because of a bug this is currently necessary

        _world = _physicsCommon.createPhysicsWorld(worldSettings);
        #ifdef ANE_DEBUG
        _world->setIsDebugRenderingEnabled(true);
        _debugRenderer = &_world->getDebugRenderer();

        _debugRenderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
        _debugRenderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);

        #endif
    }

    PhysicsSystem::~PhysicsSystem() = default;

    void PhysicsSystem::Free()
    {
        // Because reactphysics auto destructs itself we cant call out destructor
    }

    rp3d::PhysicsWorld& PhysicsSystem::GetPhysicsWorld() const
    {
        return *_world;
    }

    RigidBody* PhysicsSystem::CreateRigidBody(Entity entity)
    {
        const TransformMatrix transform = entity.GetComponent<TransformComponent>().Transform;
        const rp3d::Transform reactTransform(transform.GetPosition(), transform.GetQuaternion());

        const auto rigidBody = _world->createRigidBody(reactTransform);

        #ifdef ANE_DEBUG
        rigidBody->setIsDebugEnabled(true);
        #endif

        return new RigidBody(rigidBody);
    }

    SphereCollider* PhysicsSystem::CreateSphereCollider(Entity entity, const float radius)
    {
        if(!entity.HasComponent<RigidBodyComponent>())
        {
            ANE_ELOG_WARN("Entity has no RigidBodyComponent, adding one");
            entity.AddComponent<RigidBodyComponent>(entity);
        }

        const auto rigidBody = entity.GetComponent<RigidBodyComponent>();
        const auto collider = rigidBody.GetRigidBody()->GetReactRigidBody().addCollider(CreateSphereShape(radius), rp3d::Transform::identity());

        return new SphereCollider(collider);
    }

    BoxCollider* PhysicsSystem::CreateBoxCollider(Entity entity, const Vector3& halfExtents)
    {
        if(!entity.HasComponent<RigidBodyComponent>())
        {
            ANE_ELOG_WARN("Entity has no RigidBodyComponent, adding one");
            entity.AddComponent<RigidBodyComponent>(entity);
        }

        const auto rigidBody = entity.GetComponent<RigidBodyComponent>();
        const auto collider = rigidBody.GetRigidBody()->GetReactRigidBody().addCollider(CreateBoxShape(halfExtents), rp3d::Transform::identity());

        return new BoxCollider(collider);
    }

    CapsuleCollider* PhysicsSystem::CreateCapsuleCollider(Entity entity, const float radius, const float height)
    {
        if(!entity.HasComponent<RigidBodyComponent>())
        {
            ANE_ELOG_WARN("Entity has no RigidBodyComponent, adding one");
            entity.AddComponent<RigidBodyComponent>(entity);
        }

        const auto rigidBody = entity.GetComponent<RigidBodyComponent>();
        const auto collider = rigidBody.GetRigidBody()->GetReactRigidBody().addCollider(CreateCapsuleShape(radius, height), rp3d::Transform::identity());

        return new CapsuleCollider(collider);
    }

    void PhysicsSystem::RemoveCollider(Entity entity, const Collider* collider)
    {
        if(!entity.HasComponent<RigidBodyComponent>())
        {
            ANE_ELOG_WARN("Entity has no RigidBodyComponent, cannot remove collider");
            return;
        }

        const auto rigidBody = entity.GetComponent<RigidBodyComponent>();
        rigidBody.GetRigidBody()->GetReactRigidBody().removeCollider(&collider->GetReactCollider());
    }

    rp3d::SphereShape* PhysicsSystem::CreateSphereShape(const float radius)
    {
        const auto sphereCollider = _physicsCommon.createSphereShape(radius);
        return sphereCollider;
    }

    rp3d::BoxShape* PhysicsSystem::CreateBoxShape(const Vector3& halfExtents)
    {
        const auto boxCollider = _physicsCommon.createBoxShape(halfExtents);
        return boxCollider;
    }

    rp3d::CapsuleShape* PhysicsSystem::CreateCapsuleShape(const float radius, const float height)
    {
        const auto capsuleCollider = _physicsCommon.createCapsuleShape(radius, height);
        return capsuleCollider;
    }

    void PhysicsSystem::PhysicsUpdate(const float timeStep, Scene* scene)
    {
        bool sleepUpdate = false;

        const auto group = scene->_registry.view<TransformComponent, RigidBodyComponent>();
        for (const auto entity : group) //We need to apply changes in our transform to the internal rigidbody
        {
            auto[transform, body] = group.get<TransformComponent, RigidBodyComponent>(entity);

            TransformMatrix& transformMatrix = transform.Transform;

            if(!transformMatrix.IsDirty()) continue;

            if(!sleepUpdate) // Dirty fix for a bug that crashes when a sleeping body tries to collide
            {
                for (uint32_t i = 0; i < _world->getNbRigidBodies(); ++i)
                {
                    _world->getRigidBody(i)->setIsSleeping(false);
                }
                sleepUpdate = true;
            }
            body.GetRigidBody()->SetTransform(transformMatrix.GetPosition(), transformMatrix.GetQuaternion());
            transformMatrix.ClearDirty();
        }

        _world->update(timeStep);
    }

    void PhysicsSystem::UpdateRigidBodies(const float factor, Scene* scene)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        const auto group = scene->_registry.view<TransformComponent, RigidBodyComponent>();
        for (const auto entity : group)
        {
            auto[transform, body] = group.get<TransformComponent, RigidBodyComponent>(entity);

            TransformMatrix& transformMatrix = transform.Transform;
            const RigidBody* rigidBody = body.GetRigidBody();

            if(transformMatrix.IsDirty() || rigidBody->GetBodyType() == BodyType::Static) continue;
            if(!rigidBody->IsActive() || rigidBody->IsSleeping()) continue;

            auto currentTransform = rp3d::Transform(transformMatrix.GetPosition(), Quaternion::FromEulerAngles(transformMatrix.GetEulerAngles()));
            auto newTransform = rp3d::Transform::interpolateTransforms(currentTransform, rigidBody->GetReactRigidBody().getTransform(), factor);

            transformMatrix.SetPosition(Vector3::Convert(newTransform.getPosition()));
            transformMatrix.SetRotation(Quaternion::Convert(newTransform.getOrientation()).GetEulerAngles()); //TODO: Issues with scale and rotation
            transformMatrix.ClearDirty();
        }
    }
}
