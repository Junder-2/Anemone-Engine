#pragma once
#include "ANE/Core/Entity/ScriptableEntity.h"
#include "ANE/Physics/CollisionData.h"

namespace Engine
{
    class ScriptableEntity;

    enum class CollisionEventType;

    struct NativeScriptComponent : Component
    {
        ANE_COMPONENT_INIT(NativeScriptComponent)

        std::string ScriptName;
        bool Running = true;

        ScriptableEntity* Instance = nullptr;

        std::function<void()> Instantiate;
        std::function<void()> Destroy;

        std::function<void()> OnCreateFunction;
        std::function<void(float)> OnUpdateFunction;
        std::function<void(float)> OnFixedUpdateFunction;
        std::function<void()> OnDestroyFunction;

        std::function<void(CollisionEventType, const CollisionData&)> OnCollisionFunction;
        std::function<void(CollisionEventType, const TriggerData&)> OnTriggerFunction;

        template <typename T>
        void Bind()
        {
            Instantiate = [&]()
            {

                Instance = new T();
                ScriptName = ToString();
                const std::string prefix = "struct Engine::";

                // prefix removal
                std::string result1 = ScriptName.substr(prefix.length());
                ScriptName =  result1;
            };

            Destroy = [&]()
            {
                delete static_cast<T*>(Instance);
                Instance = nullptr;
            };

            OnCreateFunction = [&]()
            {
                static_cast<T*>(Instance)->OnCreate();
            };

            OnUpdateFunction = [&](float deltaTime)
            {
                static_cast<T*>(Instance)->OnUpdate(deltaTime);
            };
            OnFixedUpdateFunction = [&](float timeStep)
            {
                static_cast<T*>(Instance)->OnFixedUpdate(timeStep);
            };

            OnDestroyFunction = [&]()
            {
                static_cast<T*>(Instance)->OnDestroy();
            };

            OnCollisionFunction = [&](const CollisionEventType type, const CollisionData& collisionData)
            {
                static_cast<T*>(Instance)->OnCollision(type, collisionData);
            };

            OnTriggerFunction = [&](const CollisionEventType type, const TriggerData& collisionData)
            {
                static_cast<T*>(Instance)->OnTrigger(type, collisionData);
            };
        }

        static void RegisterComponentMetaData()
        {
            entt::meta<NativeScriptComponent>()
                .data<&NativeScriptComponent::ScriptName>("Script Name"_hs).prop("display_name"_hs, "Script Name")
                .NOT_EDITABLE
                .data<&NativeScriptComponent::Running>("Running"_hs).prop("display_name"_hs, "Is Running")
                .EDITABLE;
        }
    };
}
