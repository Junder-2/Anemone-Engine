#include "anepch.h"
#include "PhysicsSystem.h"

#include "PhysicsLogger.h"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Core/Math/Matrix/TransformMatrix.h"

namespace Engine
{
    PhysicsSystem::PhysicsSystem()
    {
        _physicsLogger = new PhysicsLogger();
        _physicsCommon.setLogger(_physicsLogger);
        _world = _physicsCommon.createPhysicsWorld(reactphysics3d::PhysicsWorld::WorldSettings());
    }

    PhysicsSystem::~PhysicsSystem() = default;

    void PhysicsSystem::Free()
    {
        // Because reactphysics auto destructs itself we cant call out destructor
    }

    reactphysics3d::PhysicsWorld& PhysicsSystem::GetPhysicsWorld() const
    {
        return *_world;
    }

    reactphysics3d::RigidBody& PhysicsSystem::CreateRigidBody(Entity entity)
    {
        const TransformMatrix transform = entity.GetComponent<TransformComponent>().Transform;
        const reactphysics3d::Transform reactTransform(transform.GetPosition(), transform.GetQuaternion());

        return *_world->createRigidBody(reactTransform);
    }
}
