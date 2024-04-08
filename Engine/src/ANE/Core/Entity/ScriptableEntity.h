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

        template <typename T>
        std::enable_if_t<std::is_base_of_v<Component, T>, T&> GetComponent()
        {
            T comp;
            if (_entity.TryGetComponent(comp)) return comp;

            return comp;
        }

    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(float deltaTime) {}
    private:
        friend class Scene;
        Entity _entity;
    };


}
