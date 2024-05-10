#pragma once

#include "Component.h"
#include "ANE/Physics/Types/Collider.h"

namespace Engine
{
    class Entity;
    struct Vector3;

    struct ColliderComponent : Component
    {
        ANE_COMPONENT_INIT(ColliderComponent)

        ColliderComponent(Collider* collider);
        ColliderComponent(Entity self, Vector3 halfExtents);
        ColliderComponent(Entity self, float radius);
        ColliderComponent(Entity self, float radius, float height);

        void AddCollider(Collider* collider);

        void RemoveCollider(Entity self, const Collider* collider);

        std::vector<Collider*>& GetColliders()
        {
            return _colliders;
        }

        Collider* GetCollider(const int index = 0) const;

        static void RegisterComponentMetaData()
        {
            entt::meta<ColliderComponent>()
                .data<&ColliderComponent::_colliders>("Colliders"_hs).prop("display_name"_hs, "Colliders")
                .EDITABLE;
        }

    private:
        std::vector<Collider*> _colliders;
    };
}
