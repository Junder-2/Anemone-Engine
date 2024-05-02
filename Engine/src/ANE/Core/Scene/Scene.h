#pragma once
#include "entt.hpp"

namespace reactphysics3d
{
    class PhysicsWorld;
}

namespace Engine
{
    class Event;

    class Scene : std::enable_shared_from_this<Scene>
    {
    public:
        Scene();
        Scene(const Scene&) = default;

        ~Scene() = default;

        void OnEvent(Event& e);
        void OnUpdate(float timeStep);

        void SubmitDrawCommands();

        entt::registry _registry;

    private:
        void OnFixedUpdate(float timeStep);

        friend class SceneSerializer;

    private:
        float _accumulator = 0;
        float _timeStep = 1.f/50.f;

    };
}
