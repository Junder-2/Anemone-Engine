#pragma once
#include "Layer.h"
#include <reactphysics3d/reactphysics3d.h>

namespace reactphysics3d
{
    class PhysicsWorld;
}

namespace Engine
{
    using namespace reactphysics3d;

    class PhysicsLayer : public Layer
    {
    public:
        PhysicsLayer();
        ~PhysicsLayer() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float deltaTime) override;
        void OnEvent(Event& e) override;

    private:
        PhysicsWorld* _physicsWorld;

        float _accumulator = 0;
        float _timeStep = 1/60.f;
    };
}
