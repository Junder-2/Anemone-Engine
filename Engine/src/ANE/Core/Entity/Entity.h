﻿#pragma once
#include <entt.hpp>

#include "ANE/Core/Scene/Scene.h"
#include "ANE/Core/Scene/Components/Component.h"
#include "ANE/Core/Scene/Components/TagComponent.h"
#include "ANE/Core/Scene/Components/TransformComponent.h"

namespace Engine
{
    class Entity
    {
    public:
        Entity(Scene* scene, const char* name) // only access this during creation
        {
            SceneHandle = scene;

            EntityHandle = SceneHandle->Registry.create();
            this->AddComponent<TransformComponent>();
            this->AddComponent<TagComponent>(name);
        };

        Entity(Entity&) = default;

        template <typename T, typename... Args>
        std::enable_if_t<std::is_base_of_v<Component, T>, T&> AddComponent(Args&&... args);

        template <typename T>
        std::enable_if_t<std::is_base_of_v<Component, T>> RemoveComponent();

        template <typename T>
        std::enable_if_t<std::is_base_of_v<Component, T>, bool> HasComponent();

        template <typename T>
        std::enable_if_t<std::is_base_of_v<Component, T>, bool> TryGetComponent(T& outComponent);

    private:
        template <typename T>
        std::enable_if_t<std::is_base_of_v<Component, T>, T&> GetComponent();

        entt::entity EntityHandle = {entt::null};
        Scene* SceneHandle;
    };

    /**
    * Adds a component to a an entity.
    * @param Args Forwards arguments to the T type constructor.
    * @return Returns a ref to the added component.
    */
    template <typename T, typename... Args>
    std::enable_if_t<std::is_base_of_v<Component, T>, T&> Entity::AddComponent(Args&&... args)
    {
        if (HasComponent<T>())
        {
            ANE_ENGINE_LOG_WARN("Has Component of type: {} returned existing component", typeid(T).name());
            return static_cast<std::optional<std::reference_wrapper<T>>>(GetComponent<T>()).value();
        }

        return SceneHandle->Registry.emplace<T>(EntityHandle, std::forward<Args>(args)...);
    }

    /**
    * Removes T type component from the entity if is has a component of that type.
    */
    template <typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>> Entity::RemoveComponent()
    {
        if (HasComponent<T>())
        {
            SceneHandle->Registry.remove<T>(EntityHandle);
            return;
        }
        ANE_ENGINE_LOG_WARN("Attempted to remove component of type {} that doesn't exist", typeid(T).name());
    }

    /**
    * Checks whether entity has T type component.
    * @return Return true if T type component is found.
    */
    template <typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>, bool> Entity::HasComponent()
    {
        auto component = SceneHandle->Registry.try_get<T>(EntityHandle);
        return component == nullptr ? false : true;
    }

    /**
    * Attemps to fetch component of type T
    * @param outComponent Ref out component
    * @return Returns true if component is found.
    */
    template <typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>, bool> Entity::TryGetComponent(T& outComponent)
    {
        if (HasComponent<T>())
        {
            outComponent = GetComponent<T>();
            return true;
        }

        ANE_ENGINE_LOG_WARN("Attempted to remove component of type {} that doesn't exist", typeid(T).name());
        return false;
    }

    template <typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>, T&> Entity::GetComponent()
    {
        return SceneHandle->Registry.get<T>(EntityHandle);
    }
}