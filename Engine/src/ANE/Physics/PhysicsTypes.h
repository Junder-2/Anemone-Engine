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
}
