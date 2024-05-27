#include "anepch.h"
#include "PhysicsSystem.h"

#include <ranges>

#include "CollisionData.h"
#include "CollisionListener.h"
#include "PhysicsLogger.h"
#include "PhysicsTypes.h"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Core/Scene/Components/ColliderComponent.h"
#include "ANE/Core/Scene/Components/NativeScriptComponent.h"
#include "ANE/Math/Types/TransformMatrix.h"
#include "ANE/Core/Scene/Components/RigidBodyComponent.h"
#include "ANE/Core/Scene/Components/TransformComponent.h"
#include "ANE/Math/VMath.h"
#include "ANE/Renderer/DebugRenderer.h"
#include "ANE/Utilities/ColorUtilities.h"
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

        _world = _physicsCommon.createPhysicsWorld(worldSettings);

        _collisionListener = new CollisionListener();
        _world->setEventListener(_collisionListener);

        #ifndef ANE_DIST
        _debugDisplayAlpha = .5f;
        _debugRenderer = &_world->getDebugRenderer();
        EnableDebugRendering(true);
        #endif
    }

    PhysicsSystem::~PhysicsSystem() = default;

    void PhysicsSystem::Free()
    {
        for (const auto& val : _reactCollider | std::views::values)
        {
            delete val;
        }
        _reactCollider.clear();

        for (const auto& val : _reactRigidBody | std::views::values)
        {
            delete val;
        }
        _reactRigidBody.clear();

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

        const auto reactRigidBody = _world->createRigidBody(reactTransform);
        _reactEntity.insert_or_assign(reactRigidBody->getEntity(), entity);

        #ifndef ANE_DIST
        reactRigidBody->setIsDebugEnabled(IsDebugRendering());
        #endif

        auto* rigidBody = new RigidBody(reactRigidBody);
        _reactRigidBody.insert_or_assign(reactRigidBody->getEntity(), rigidBody);

        return rigidBody;
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
        auto* sphereCollider = new SphereCollider(collider);

        _reactCollider.insert_or_assign(collider->getEntity(), sphereCollider);

        return sphereCollider;
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
        auto* boxCollider = new BoxCollider(collider);

        _reactCollider.insert_or_assign(collider->getEntity(), boxCollider);

        return boxCollider;
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
        auto* capsuleCollider = new CapsuleCollider(collider);

        _reactCollider.insert_or_assign(collider->getEntity(), capsuleCollider);

        return capsuleCollider;
    }

    SphereCollider* PhysicsSystem::CreateSphereCollider(const rp3d::Entity reactEntity, const float radius)
    {
        if(!_reactEntity.contains(reactEntity))
        {
            ANE_ELOG_WARN("Invalid reactEntity");
            return nullptr;
        }

        return CreateSphereCollider(_reactEntity[reactEntity], radius);
    }

    BoxCollider* PhysicsSystem::CreateBoxCollider(const rp3d::Entity reactEntity, const Vector3& halfExtents)
    {
        if(!_reactEntity.contains(reactEntity))
        {
            ANE_ELOG_WARN("Invalid reactEntity");
            return nullptr;
        }

        return CreateBoxCollider(_reactEntity[reactEntity], halfExtents);
    }

    CapsuleCollider* PhysicsSystem::CreateCapsuleCollider(const rp3d::Entity reactEntity, const float radius, const float height)
    {
        if(!_reactEntity.contains(reactEntity))
        {
            ANE_ELOG_WARN("Invalid reactEntity");
            return nullptr;
        }

        return CreateCapsuleCollider(_reactEntity[reactEntity], radius, height);
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

    void PhysicsSystem::RemoveCollider(const rp3d::Entity reactEntity, const Collider* collider)
    {
        if(!_reactEntity.contains(reactEntity))
        {
            ANE_ELOG_WARN("Invalid reactEntity");
            return;
        }

        RemoveCollider(_reactEntity[reactEntity], collider);
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

    rp3d::Entity PhysicsSystem::GetBodyEntity(Entity entity)
    {
        if(!entity.HasComponent<RigidBodyComponent>())
        {
            ANE_ELOG_WARN("Entity has no RigidBodyComponent, adding one");
            entity.AddComponent<RigidBodyComponent>(entity);
        }

        return entity.GetComponent<RigidBodyComponent>().GetRigidBody()->GetReactRigidBody().getEntity();
    }

    Entity PhysicsSystem::GetOwnerEntity(const rp3d::Entity reactEntity)
    {
        if(!_reactEntity.contains(reactEntity))
        {
            ANE_ELOG_WARN("Invalid reactEntity");
            return {};
        }

        return _reactEntity[reactEntity];
    }

    RigidBody* PhysicsSystem::ConvertRigidBody(const rp3d::Entity reactEntity)
    {
        if(!_reactRigidBody.contains(reactEntity))
        {
            ANE_ELOG_WARN("Invalid reactEntity");
            return nullptr;
        }

        return _reactRigidBody[reactEntity];
    }

    Collider* PhysicsSystem::ConvertCollider(const rp3d::Entity reactEntity)
    {
        if(!_reactCollider.contains(reactEntity))
        {
            ANE_ELOG_WARN("Invalid reactEntity");
            return nullptr;
        }

        return _reactCollider[reactEntity];
    }

    void PhysicsSystem::WakeBodies()
    {
        if(_hasAwokenBodies) return;

        // Dirty fix for a bug that crashes when a sleeping body tries to collide
        for (uint i = 0; i < _world->getNbRigidBodies(); ++i)
        {
            _world->getRigidBody(i)->setIsSleeping(false);
        }

        _hasAwokenBodies = true;
    }

    void PhysicsSystem::PhysicsUpdate(const float timeStep, Scene* scene)
    {
        std::vector<RigidBody*> oneFrameDisable; // Dirty fix for a bug that crashes for certain changes of rigidbody

        const auto group = scene->_registry.view<TransformComponent, RigidBodyComponent>();
        for (const auto entity : group) //We need to apply changes in our transform to the internal rigidbody
        {
            auto[transform, body] = group.get<TransformComponent, RigidBodyComponent>(entity);

            TransformMatrix& transformMatrix = transform.Transform;
            RigidBody* rigidBody = body.GetRigidBody();

            if(transformMatrix.IsDirty())
            {
                WakeBodies();

                if(transformMatrix.GetDirtyFlags() & DirtyScale)
                {
                    if(const auto colliderComp = scene->_registry.try_get<ColliderComponent>(entity))
                    {
                        for (const auto collider : colliderComp->GetColliders())
                        {
                            collider->SetScale(transformMatrix.GetScale());
                        }
                    }
                }
                body.GetRigidBody()->SetTransform(transformMatrix.GetPosition(), transformMatrix.GetQuaternion());

                transformMatrix.ClearDirty();
            }
            if(rigidBody->IsDirty())
            {
                WakeBodies();
                rigidBody->TryUpdate();

                if(rigidBody->IsActive())
                {
                    rigidBody->SetActive(false);
                    oneFrameDisable.push_back(rigidBody);
                }
            }
        }

        _world->update(timeStep);

        for (const auto rb : oneFrameDisable)
        {
            rb->SetActive(true);
            rb->SetPosition(rb->GetPosition());
            rb->ClearDirty();
        }

        _hasAwokenBodies = false;
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

            const auto currentTransform = rp3d::Transform(transformMatrix.GetPosition(), transformMatrix.GetQuaternion());
            const auto targetTransform = rigidBody->GetReactRigidBody().getTransform();
            if(currentTransform == targetTransform) continue;
            auto newTransform = rp3d::Transform::interpolateTransforms(currentTransform, targetTransform, factor);

            if(currentTransform.getPosition() != targetTransform.getPosition()) transformMatrix.SetPosition(Vector3::Convert(newTransform.getPosition()));
            if(currentTransform.getOrientation() != targetTransform.getOrientation()) transformMatrix.SetRotation(Quaternion::Convert(newTransform.getOrientation())); //Todo: still some problems with scale
            transformMatrix.ClearDirty();
        }

        #ifndef ANE_DIST
        DebugDraw();
        #endif
    }

    void PhysicsSystem::DispatchCollisionCallback(Entity entity, const CollisionEventType type, const CollisionData& collisionData)
    {
        if(entity.HasComponent<NativeScriptComponent>())
        {
            entity.GetComponent<NativeScriptComponent>().OnCollisionFunction(type, collisionData);
        }
    }

    void PhysicsSystem::DispatchTriggerCallback(Entity entity, const CollisionEventType type, const TriggerData& triggerData)
    {
        if(entity.HasComponent<NativeScriptComponent>())
        {
            entity.GetComponent<NativeScriptComponent>().OnTriggerFunction(type, triggerData);
        }
    }

    #ifndef ANE_DIST
    void PhysicsSystem::EnableDebugRendering(const bool enable)
    {
        _isDebugRendering = enable;
        _world->setIsDebugRenderingEnabled(_isDebugRendering);

        for (uint i = 0; i < _world->getNbRigidBodies(); ++i)
        {
            _world->getRigidBody(i)->setIsDebugEnabled(_isDebugRendering);
        }
    }

    void PhysicsSystem::EnableDebugFlag(PhysicsDebugDisplayFlag displayFlag, const bool enable) const
    {
        _debugRenderer->setIsDebugItemDisplayed(static_cast<rp3d::DebugRenderer::DebugItem>(displayFlag), enable);
    }

    bool PhysicsSystem::IsDebugDisplayFlag(PhysicsDebugDisplayFlag displayFlag) const
    {
        return _debugRenderer->getIsDebugItemDisplayed(static_cast<rp3d::DebugRenderer::DebugItem>(displayFlag));
    }

    void PhysicsSystem::DebugDraw()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if(!_isDebugRendering) return;

        if(_debugRenderer->getNbTriangles() > 0)
        {
            for (auto triangle : _debugRenderer->getTriangles())
            {
                DebugRenderer::PushColor(Vector4(ColorUtilities::HexToRGB(triangle.color1), _debugDisplayAlpha));
                DebugRenderer::DrawTriangle(Vector3::Convert(triangle.point1), Vector3::Convert(triangle.point2), Vector3::Convert(triangle.point3));
            }
        }
        if(_debugRenderer->getNbLines() > 0)
        {
            for (auto line : _debugRenderer->getLines())
            {
                DebugRenderer::PushColor(Vector4(ColorUtilities::HexToRGB(line.color1), _debugDisplayAlpha));
                DebugRenderer::DrawLine(Vector3::Convert(line.point1), Vector3::Convert(line.point2));
            }
        }
    }
    #endif
}
