#pragma once

#include "ANE/Core/Entity/Entity.h"
#include "ANE/Core/Entity/ScriptableEntity.h"

namespace Engine
{
    class ScriptableEntity;

    struct NativeScriptComponent : Component
    {
        ANE_COMPONENT_INIT(NativeScriptComponent)

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
    };

}
