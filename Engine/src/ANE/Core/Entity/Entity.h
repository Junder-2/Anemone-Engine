﻿#pragma once

#include "ANE/Core/Scene/Scene.h"
#include "ANE/Core/Scene/Components/TagComponent.h"

namespace Engine
{
    struct Component;

    class Entity
    {
    public:
        Entity(Scene* scene, const char* name); // only access this during creation

        Entity(entt::entity handle, Scene* scene) : EntityHandle(handle), SceneHandle(scene) {}

        Entity(const Entity& other) = default;

        Entity() = default;

        //Entity(Entity&) = default;

        template <typename T, typename... Args>
        std::enable_if_t<std::is_base_of_v<Component, T>, T&> AddComponent(Args&&... args);

        template <typename T>
        std::enable_if_t<std::is_base_of_v<Component, T>> RemoveComponent();

        template <typename T>
        std::enable_if_t<std::is_base_of_v<Component, T>, bool> HasComponent();

        template <typename T>
        std::enable_if_t<std::is_base_of_v<Component, T>, bool> TryGetComponent(T& outComponent);

        operator bool() const { return EntityHandle != entt::null; }
        operator entt::entity() const { return EntityHandle; }
        operator uint32_t() const { return static_cast<uint32_t>(EntityHandle); }

        bool operator==(const Entity& other) const
        {
            return EntityHandle == other.EntityHandle && SceneHandle == other.SceneHandle;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }

        template <typename T>
        std::enable_if_t<std::is_base_of_v<Component, T>, T&> GetComponent();
    private:

        entt::entity EntityHandle = {entt::null};
        Scene* SceneHandle = nullptr;
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
            ANE_ELOG_WARN("Has Component of type: {} returned existing component", typeid(T).name());
            return static_cast<std::optional<std::reference_wrapper<T>>>(GetComponent<T>()).value();
        }

        return SceneHandle->_registry.emplace<T>(EntityHandle, std::forward<Args>(args)...);;
    }

    /**
    * Removes T type component from the entity if is has a component of that type.
    */
    template <typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>> Entity::RemoveComponent()
    {
        if (typeid(T) == typeid(TagComponent)) return;

        if (HasComponent<T>())
        {
            SceneHandle->_registry.remove<T>(EntityHandle);
            return;
        }
        ANE_ELOG_WARN("Attempted to remove component of type {} that doesn't exist", typeid(T).name());
    }

    /**
    * Checks whether entity has T type component.
    * @return Return true if T type component is found.
    */
    template <typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>, bool> Entity::HasComponent()
    {
        auto component = SceneHandle->_registry.try_get<T>(EntityHandle);
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

       // ANE_ELOG_WARN("Attempted to try get component of type {} that doesn't exist", typeid(T).name());
        return false;
    }
    template<typename Type>
    Type & get(entt::registry &registry, entt::entity entity) {
        return registry.get_or_emplace<Type>(entity);
    }

    template<typename Type>
    Type & set(entt::registry &registry, entt::entity entity, const Type &instance) {
        return registry.emplace_or_replace<Type>(entity, instance);
    }

    template<typename Type>
    void extend_meta_type() {
        entt::meta<Type>()
            .template func<&get<Type>, entt::as_ref_t>("get")
            .template func<&set<Type>>("set");
    }


    template <typename T>
    std::enable_if_t<std::is_base_of_v<Component, T>, T&> Entity::GetComponent()
    {
        return SceneHandle->_registry.get<T>(EntityHandle);
    }

}
