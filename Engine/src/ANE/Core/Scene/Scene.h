﻿#pragma once
#include "entt.hpp"
//#include "../Entity/Entity.h"

namespace reactphysics3d
{
    class PhysicsWorld;
}

namespace Engine
{
    class Event;
    class Entity;

    class Scene : std::enable_shared_from_this<Scene>
    {
    public:
        Scene();
        Scene(const Scene&) = default;

        ~Scene() = default;

        void OnEvent(Event& e);
        void OnUpdate(float timeStep);

        void SubmitDrawCommands();

        Entity Create(const char* name = "Untagged");
        Entity Create(std::string = "Untagged");
        Entity GetEntityWithUUID(std::string UUID);
        entt::registry _registry;

    private:
        void OnFixedUpdate(float timeStep);
        void UpdateRigidBodies();

    private:
        friend class Entity;

        reactphysics3d::PhysicsWorld* _physicsWorld;

        float _accumulator = 0;
        float _timeStep = 1.f/50.f;

        std::map<std::string, Engine::Entity> _entityMap;
    };
}
