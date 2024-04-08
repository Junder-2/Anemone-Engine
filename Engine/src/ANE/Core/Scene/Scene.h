﻿#pragma once
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

    private:
        entt::registry _registry;
        friend class Entity;
    };
}
