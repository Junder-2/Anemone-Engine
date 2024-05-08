#include "anepch.h"
#include "ColliderComponent.h"

#include "ANE/Core/Entity/Entity.h"
#include "ANE/Math/Types/Vector3.h"
#include "ANE/Physics/Physics.h"

namespace Engine
{
    ColliderComponent::ColliderComponent(Collider* collider)
    {
        _colliders.push_back(collider);
    }

    ColliderComponent::ColliderComponent(const Entity self, const Vector3 halfExtents): Component(typeid(*this).name())
    {
        _colliders.push_back(GetPhysicsSystem().CreateBoxCollider(self, halfExtents));
    }

    ColliderComponent::ColliderComponent(const Entity self, const float radius): Component(typeid(*this).name())
    {
        _colliders.push_back(GetPhysicsSystem().CreateSphereCollider(self, radius));
    }

    ColliderComponent::ColliderComponent(const Entity self, const float radius, const float height): Component(typeid(*this).name())
    {
        _colliders.push_back(GetPhysicsSystem().CreateCapsuleCollider(self, radius, height));
    }

    void ColliderComponent::AddCollider(Collider* collider)
    {
        _colliders.push_back(collider);
    }

    void ColliderComponent::RemoveCollider(const Entity self, const Collider* collider)
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

    Collider* ColliderComponent::GetCollider(const int index) const
    {
        if(_colliders.size() < index)
        {
            ANE_ELOG_WARN("Trying to access collider at invalid index {0}", index);
            return nullptr;
        }
        return _colliders[index];
    }
}


