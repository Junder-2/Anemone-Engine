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
        AddBoxCollider(self, halfExtents);
    }

    ColliderComponent::ColliderComponent(const Entity self, const float radius): Component(typeid(*this).name())
    {
        AddSphereCollider(self, radius);
    }

    ColliderComponent::ColliderComponent(const Entity self, const float radius, const float height): Component(typeid(*this).name())
    {
        AddCapsuleCollider(self, radius, height);
    }

    Collider* ColliderComponent::AddCollider(Collider* collider)
    {
        _colliders.push_back(collider);
        return collider;
    }

    BoxCollider* ColliderComponent::AddBoxCollider(const Entity self, const Vector3 halfExtents)
    {
        BoxCollider* col = GetPhysicsSystem().CreateBoxCollider(self, halfExtents);
        _colliders.push_back(col);
        return col;
    }

    SphereCollider* ColliderComponent::AddSphereCollider(const Entity self, const float radius)
    {
        SphereCollider* col = GetPhysicsSystem().CreateSphereCollider(self, radius);
        _colliders.push_back(col);
        return col;
    }

    CapsuleCollider* ColliderComponent::AddCapsuleCollider(const Entity self, const float radius, const float height)
    {
        CapsuleCollider* col = GetPhysicsSystem().CreateCapsuleCollider(self, radius, height);
        _colliders.push_back(col);
        return col;
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


