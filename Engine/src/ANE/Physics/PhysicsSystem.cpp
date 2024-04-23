#include "anepch.h"
#include "PhysicsSystem.h"

#include "PhysicsLogger.h"

#include "ANE/Math/Types/TransformMatrix.h"

namespace Engine
{
    PhysicsSystem::PhysicsSystem()
    {
        _physicsLogger = new PhysicsLogger();
        _physicsCommon.setLogger(_physicsLogger);
        _world = _physicsCommon.createPhysicsWorld(PhysicsWorld::WorldSettings());
    }

    PhysicsSystem::~PhysicsSystem()
    {
        _physicsCommon.destroyPhysicsWorld(_world);
        delete _physicsLogger;
    }

    RigidBody& PhysicsSystem::CreateRigidBody(const TransformMatrix& transformMatrix)
    {
        const Transform transform(transformMatrix.GetPosition(), transformMatrix.GetQuaternion());

        return *_world->createRigidBody(transform);
    }
}
