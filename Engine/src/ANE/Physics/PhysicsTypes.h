#pragma once
#include "ANE/Math/MathCommon.h"

namespace Engine
{
    const float MIN_SCALE = FMath::EPSILON;

    typedef uint16_t CollisionLayerMask;

    enum class CollisionLayer : uint16_t
    {
        All = UINT16_MAX,
        Static = BIT(0),
        Dynamic = BIT(1),
        Player = BIT(2),
        Layer4 = BIT(3),
        Layer5 = BIT(4),
        Layer6 = BIT(5),
        Layer7 = BIT(6),
        Layer8 = BIT(7),
        Layer9 = BIT(8),
        Layer10 = BIT(9),
        Layer11 = BIT(10),
        Layer12 = BIT(11),
        Layer13 = BIT(12),
        Layer14 = BIT(13),
        Layer15 = BIT(14),
        Layer16 = BIT(15),
    };
    ENUM_CLASS_OPERATORS(CollisionLayer)

    enum class CollisionLayerPreset : uint16_t
    {
        All = CollisionLayer::All,
        Default = CollisionLayer::Static | CollisionLayer::Dynamic | CollisionLayer::Player,
    };

    enum class CollisionEventType
    {
        Enter,
        Stay,
        Exit
    };

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
    ENUM_CLASS_OPERATORS(PhysicsDebugDisplayFlag)

}
