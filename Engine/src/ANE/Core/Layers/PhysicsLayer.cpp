#include "anepch.h"
#include "PhysicsLayer.h"

#include "ANE/Physics/Physics.h"

namespace Engine
{
    PhysicsLayer::PhysicsLayer()
    {
        _physicsWorld = &GetPhysicsSystem().GetPhysicsWorld();
    }

    PhysicsLayer::~PhysicsLayer()
    {
    }

    void PhysicsLayer::OnAttach()
    {

    }

    void PhysicsLayer::OnDetach()
    {

    }

    void PhysicsLayer::OnUpdate(float deltaTime)
    {
        _accumulator += deltaTime;

        // Fixed update
        while (_accumulator >= _timeStep)
        {
            _physicsWorld->update(_timeStep);

            _accumulator -= _timeStep;
        }
    }

    void PhysicsLayer::OnEvent(Event& e)
    {
        Layer::OnEvent(e);
    }
}
