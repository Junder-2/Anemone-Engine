#include "anepch.h"
#include "ColliderComponent.h"

#include "ANE/Core/Entity/Entity.h"
#include "ANE/Math/Types/Vector3.h"
#include "ANE/Physics/Physics.h"

namespace Engine
{
    ColliderComponent::ColliderComponent(Collider* collider) : Component(typeid(*this).name())
    {
        _data.BodyId = collider->GetReactCollider().getBody()->getEntity();
        _data.Colliders.push_back(collider);
    }

    ColliderComponent::ColliderComponent(const Entity self) : Component(typeid(*this).name())
    {
        Init(self);
    }

    ColliderComponent::ColliderComponent(const Entity self, const Vector3 halfExtents) : Component(typeid(*this).name())
    {
        Init(self);
        AddBoxCollider(halfExtents);
    }

    ColliderComponent::ColliderComponent(const Entity self, const float radius) : Component(typeid(*this).name())
    {
        Init(self);
        AddSphereCollider(radius);
    }

    ColliderComponent::ColliderComponent(const Entity self, const float radius, const float height) : Component(typeid(*this).name())
    {
        Init(self);
        AddCapsuleCollider(radius, height);
    }

    Collider* ColliderComponent::AddCollider(Collider* collider)
    {
        _data.Colliders.push_back(collider);
        return collider;
    }

    BoxCollider* ColliderComponent::AddBoxCollider(const Vector3 halfExtents)
    {
        BoxCollider* col = GetPhysicsSystem().CreateBoxCollider(_data.BodyId, halfExtents);
        _data.Colliders.push_back(col);
        return col;
    }

    SphereCollider* ColliderComponent::AddSphereCollider(const float radius)
    {
        SphereCollider* col = GetPhysicsSystem().CreateSphereCollider(_data.BodyId, radius);
        _data.Colliders.push_back(col);
        return col;
    }

    CapsuleCollider* ColliderComponent::AddCapsuleCollider(const float radius, const float height)
    {
        CapsuleCollider* col = GetPhysicsSystem().CreateCapsuleCollider(_data.BodyId, radius, height);
        _data.Colliders.push_back(col);
        return col;
    }

    void ColliderComponent::RemoveCollider(const Collider* collider)
    {
        if(const auto element = std::ranges::find(_data.Colliders, collider); element != _data.Colliders.end())
        {
            _data.Colliders.erase(element);
        }
        else
        {
            ANE_ELOG_WARN("Trying to remove collider, but its not inside of list");
            return;
        }

        GetPhysicsSystem().RemoveCollider(_data.BodyId, collider);
    }

    Collider* ColliderComponent::GetCollider(const int index) const
    {
        if(_data.Colliders.size() < index)
        {
            ANE_ELOG_WARN("Trying to access collider at invalid index {0}", index);
            return nullptr;
        }
        return _data.Colliders[index];
    }

    void ColliderComponent::Init(const Entity self)
    {
        _data.BodyId = GetPhysicsSystem().GetBodyEntity(self);
    }
}


