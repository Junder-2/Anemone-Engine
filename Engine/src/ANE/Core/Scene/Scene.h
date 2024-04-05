#pragma once
#include "../../Events/Event.h"
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

        //Scene(const std::vector<entt::entity>& entities) : _entities(entities) { }

        ~Scene() = default;

        void OnEvent(Event& e);
        void OnUpdate(float timeStep);

        // std::vector<entt::entity> GetEntities()
        // {
        //     return _entities;
        // };

        Entity Create(const char* name = "Untagged");

        // operator const std::vector<entt::entity>() const { return _entities; }
        // operator std::vector<entt::entity>() { return _entities; }

        entt::registry Registry;
    private:
       // std::vector<entt::entity> _entities; // todo: this might not be used.

        friend class Entity;
    };
}
