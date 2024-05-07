#pragma once
#include "entt.hpp"

namespace Engine
{
    class Event;
    class Entity;

    class Scene : std::enable_shared_from_this<Scene>
    {
    public:
        Scene(std::string sceneName);
        Scene(const Scene&) = default;
        ~Scene() = default;

        void OnEvent(Event& e);
        void OnUpdate(float timeStep);

        Entity Create(const char* name = "Untagged");
        Entity Create(std::string = "Untagged");
        Entity GetEntityWithUUID(std::string UUID);

        void SubmitDrawCommands();

        entt::registry _registry;

        std::string Name;
    private:

        std::map<std::string, Entity> _entityMap;
        void OnFixedUpdate(float timeStep);

    private:
        float _accumulator = 0;
        float _timeStep = 1.f/50.f;
    };
}
