#pragma once

#include "Component.h"

namespace Engine
{
    class Entity;
    struct Vector3;
    class Collider;
    class BoxCollider;
    class SphereCollider;
    class CapsuleCollider;

    struct ColliderComponentData
    {
        rp3d::Entity BodyId {256, 0};
        std::vector<Collider*> Colliders;
    };

    struct ColliderComponent : Component
    {
        ANE_COMPONENT_INIT(ColliderComponent)

        ColliderComponent(Collider* collider);
        ColliderComponent(Entity self);
        ColliderComponent(Entity self, Vector3 halfExtents);
        ColliderComponent(Entity self, float radius);
        ColliderComponent(Entity self, float radius, float height);

        Collider* AddCollider(Collider* collider);
        BoxCollider* AddBoxCollider(Vector3 halfExtents);
        SphereCollider* AddSphereCollider(float radius);
        CapsuleCollider* AddCapsuleCollider(float radius, float height);

        void RemoveCollider(const Collider* collider);

        std::vector<Collider*>& GetColliders()
        {
            return _data.Colliders;
        }

        Collider* GetCollider(int index = 0) const;
        int GetColliderCount() const { return _data.Colliders.size(); }

        static void RegisterComponentMetaData()
        {
            entt::meta<ColliderComponent>()
                .data<&ColliderComponent::_data>("Colliders"_hs).prop("display_name"_hs, "Colliders")
                .EDITABLE;
        }

    private:
        void Init(Entity self);

    private:
        ColliderComponentData _data;
    };
}
