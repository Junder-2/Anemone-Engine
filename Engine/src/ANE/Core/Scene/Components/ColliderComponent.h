#pragma once

#include "Component.h"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Physics/Physics.h"
#include "ANE/Physics/Types/Collider.h"
#include "ANE/Physics/Types/BoxCollider.h"
#include "ANE/Physics/Types/SphereCollider.h"
#include "ANE/Physics/Types/CapsuleCollider.h"

namespace Engine
{
    struct ColliderComponent : Component
    {
    public:
        ANE_COMPONENT_INIT(ColliderComponent)

        ColliderComponent(Collider* collider) : Component(typeid(*this).name())
        {
            _colliders.push_back(collider);
        }

        ColliderComponent(const Entity self, const Vector3 halfExtents) : Component(typeid(*this).name())
        {
            _colliders.push_back(GetPhysicsSystem().CreateBoxCollider(self, halfExtents));
        }

        ColliderComponent(const Entity self, const float radius) : Component(typeid(*this).name())
        {
            _colliders.push_back(GetPhysicsSystem().CreateSphereCollider(self, radius));
        }

        ColliderComponent(const Entity self, const float radius, const float height) : Component(typeid(*this).name())
        {
            _colliders.push_back(GetPhysicsSystem().CreateCapsuleCollider(self, radius, height));
        }

        void AddCollider(Collider* collider)
        {
            _colliders.push_back(collider);
        }

        void RemoveCollider(const Entity self, const Collider* collider)
        {
            if(const auto element = std::ranges::find(_colliders, collider); element != _colliders.end())
            {
                _colliders.erase(element);
            }
            else
            {
                ANE_ELOG_WARN("Trying to remove collider, but its not inside of list");
                return;
            }

            GetPhysicsSystem().RemoveCollider(self, collider);
        }

        std::vector<Collider*>& GetColliders()
        {
            return _colliders;
        }

        Collider* GetCollider(const int index = 0) const
        {
            if(_colliders.size() < index)
            {
                ANE_ELOG_WARN("Trying to access collider at invalid index {0}", index);
                return nullptr;
            }
            return _colliders[index];
        }

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
