#pragma once

namespace Engine
{
    enum class CollisionShapeType
    {
        Sphere,
        Box,
        Capsule
    };

    enum class BodyMode
    {
        Static,
        Kinematic,
        Dynamic
    };
}
