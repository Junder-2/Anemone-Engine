#pragma once

#include "Entity.h"

namespace Engine
{
    class ScriptableEntity
    {
    public:
        //virtual ~ScriptableEntity();

        //ScriptableEntity(Entity entity) : _entity(entity) {  }
        virtual ~ScriptableEntity() = default;

        /**
         * \brief Attempt to Get component from Entity, if unsure whether is has component, use TryGetComponent()
         * \tparam T Type of the component you want to grab
         * \return returns default if no component if found
         */
        template <typename T>
        std::enable_if_t<std::is_base_of_v<Component, T>, T&> [[nodiscard("Component not ")]] GetComponent()
        {
            //T& comp = default;
            if (!_entity.HasComponent<T>())
            {
                if (TagComponent outComponent; _entity.TryGetComponent(outComponent))
                    ANE_ELOG_WARN("No Component of type: {0} found on Entity: {1}", typeid(T).name(), outComponent.Value);
            }

            return _entity.GetComponent<T>();
        }

        /**
         * \brief Attempt to Get component from Entity.
         * \tparam T Type of the component you want to grab
         * \param outComponent Component ref that gets set
         * \return returns true if component is found on the entity
         */
        template <typename T>
        std::enable_if_t<std::is_base_of_v<Component, T>, bool> TryGetComponent(T& outComponent)
        {
            _entity.TryGetComponent(outComponent);

            return _entity.HasComponent<T>();
        }

    protected:
        virtual void OnCreate()
        {
        }

        virtual void OnDestroy()
        {
        }

        virtual void OnUpdate(float deltaTime)
        {
        }

    private:
        friend class Scene;
        Entity _entity;
    };
}
