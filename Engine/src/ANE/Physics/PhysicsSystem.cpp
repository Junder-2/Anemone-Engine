#include "anepch.h"
#include "PhysicsSystem.h"

#include "PhysicsLogger.h"
#include "ANE/Core/Math/TransformMatrix.h"
#include "ANE/Utilities/PhysicsUtilities.h"

namespace Engine
{
    PhysicsSystem::PhysicsSystem()
    {
        _physicsLogger = new PhysicsLogger();
        _physicsCommon.setLogger(_physicsLogger);
        _world = std::unique_ptr<PhysicsWorld>(_physicsCommon.createPhysicsWorld(PhysicsWorld::WorldSettings()));
    }

    PhysicsSystem::~PhysicsSystem()
    {
        _physicsCommon.destroyPhysicsWorld(_world.get());
        delete _physicsLogger;
    }

    RigidBody& PhysicsSystem::CreateRigidBody(const TransformMatrix& transformMatrix)
    {
        Vector3 vec = transformMatrix.GetPosition();

        // Transform transform(transformMatrix.GetPosition(), transformMatrix.GetRotation());
        // _world->createRigidBody()
    }
}
