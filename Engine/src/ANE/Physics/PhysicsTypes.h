#pragma once

namespace Engine
{
    const float MIN_SCALE = 0.01f;

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
        ColliderAABB                = 1 << 0,
        ColliderBroadphaseAABB      = 1 << 1,
        CollisionShape              = 1 << 2,
        ContactPoint                = 1 << 3,
        ContactNormal               = 1 << 4,
        CollisionShapeNormal        = 1 << 5,
    };
}
