#pragma once
#include "ANE/Events/Event.h"
//#include "../Entity/Entity.h"
#include "entt.hpp"

namespace Engine
{
    class Entity;

    class Scene : std::enable_shared_from_this<Scene>
    {

    public:
        Scene();
        Scene(const Scene&) = default;

        ~Scene() = default;

        void OnEvent(Event& e);
        void OnUpdate(float timeStep);

        Entity Create(const char* name = "Untagged");
        Entity Create(std::string = "Untagged");
        Entity GetEntityWithUUID(std::string UUID);
        entt::registry _registry;

    private:
        friend class Entity;

        std::map<std::string, Entity> _entityMap;

    };
}
