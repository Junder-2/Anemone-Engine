#pragma once
#include "ANE/Math/MathCommon.h"

namespace Engine
{
    const float MIN_SCALE = FMath::EPSILON;

    enum class CollisionShapeType
    {
        Sphere,
        Box,
        Capsule
    };

    enum class BodyType
    {
        Static,
        Kinematic,
        Dynamic
    };

    enum class PhysicsDebugDisplayFlag
    {
        ColliderAABB                = BIT(0),
        ColliderBroadphaseAABB      = BIT(1),
        CollisionShape              = BIT(2),
        ContactPoint                = BIT(3),
        ContactNormal               = BIT(4),
        CollisionShapeNormal        = BIT(5),
    };
}
