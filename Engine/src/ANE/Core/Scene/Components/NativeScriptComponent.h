#pragma once

#include "ANE/Core/Entity/ScriptableEntity.h"

namespace Engine
{
    class ScriptableEntity;

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
