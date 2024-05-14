#pragma once
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Math/Types/Vector3.h"

namespace Engine
{
    class Collider;
    class RigidBody;

    struct ContactData
    {
        Vector3 Normal;
        Vector3 Point;
        float PenetrationDepth;
    };

    struct CollisionData
    {
        RigidBody* RigidBody;
        Collider* Collider;
        Entity Entity;
        std::vector<ContactData> Contacts;
    };

    struct TriggerData
    {
        RigidBody* RigidBody;
        Collider* Collider;
        Entity Entity;
    };
}
