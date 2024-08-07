#pragma once
#include "FMath.h"
#include "Types/Vector2.h"
#include "Types/Vector3.h"
#include "Types/Vector4.h"

namespace Engine::Math
{
    /**
     * Returns the absolute value
     */
    inline Vector2 Abs(const Vector2 x)
    {
        return {FMath::Abs(x.X), FMath::Abs(x.Y) };
    }

    /**
     * Returns the absolute value
     */
    inline Vector3 Abs(const Vector3 x)
    {
        return {FMath::Abs(x.X), FMath::Abs(x.Y), FMath::Abs(x.Z) };
    }

    /**
     * Returns the absolute value
     */
    inline Vector4 Abs(const Vector4 x)
    {
        return {FMath::Abs(x.X), FMath::Abs(x.Y), FMath::Abs(x.Z), FMath::Abs(x.W) };
    }

    /**
     * Returns the signs of the value
     */
    inline Vector2 Sign(const Vector2 x)
    {
        return {FMath::Sign(x.X), FMath::Sign(x.Y)};
    }

    /**
     * Returns the signs of the value
     */
    inline Vector3 Sign(const Vector3 x)
    {
        return {FMath::Sign(x.X), FMath::Sign(x.Y), FMath::Sign(x.Z)};
    }

    /**
     * Returns the signs of the value
     */
    inline Vector4 Sign(const Vector4 x)
    {
        return {FMath::Sign(x.X), FMath::Sign(x.Y), FMath::Sign(x.Z), FMath::Sign(x.W)};
    }

    /**
     * Returns the smallest of the two values
     */
    inline Vector2 Min(const Vector2 x, const Vector2 y)
    {
        return {FMath::Min(x.X, y.X), FMath::Min(x.Y, y.Y)};
    }

    /**
     * Returns the smallest of the two values
     */
    inline Vector3 Min(const Vector3 x, const Vector3 y)
    {
        return {FMath::Min(x.X, y.X), FMath::Min(x.Y, y.Y), FMath::Min(x.Z, y.Z)};
    }

    /**
     * Returns the smallest of the two values
     */
    inline Vector4 Min(const Vector4 x, const Vector4 y)
    {
        return {FMath::Min(x.X, y.X), FMath::Min(x.Y, y.Y), FMath::Min(x.Z, y.Z), FMath::Min(x.W, y.W)};
    }

    /**
     * Returns the biggest of the two values
     */
    inline Vector2 Max(const Vector2 x, const Vector2 y)
    {
        return {FMath::Max(x.X, y.X), FMath::Max(x.Y, y.Y)};
    }

    /**
     * Returns the biggest of the two values
     */
    inline Vector3 Max(const Vector3 x, const Vector3 y)
    {
        return {FMath::Max(x.X, y.X), FMath::Max(x.Y, y.Y), FMath::Max(x.Z, y.Z)};
    }
    /**
     * Returns the biggest of the two values
     */
    inline Vector4 Max(const Vector4 x, const Vector4 y)
    {
        return {FMath::Max(x.X, y.X), FMath::Max(x.Y, y.Y), FMath::Max(x.Z, y.Z), FMath::Max(x.W, y.W)};
    }

    /**
     * Clamps a value between min and max
     */
    inline Vector2 Clamp(const Vector2 x, const Vector2 min, const Vector2 max)
    {
        return {FMath::Clamp(x.X, min.X, max.X), FMath::Clamp(x.Y, min.Y, max.Y) };
    }

    /**
     * Clamps a value between min and max
     */
    inline Vector3 Clamp(const Vector3 x, const Vector3 min, const Vector3 max)
    {
        return {FMath::Clamp(x.X, min.X, max.X), FMath::Clamp(x.Y, min.Y, max.Y), FMath::Clamp(x.Z, min.Z, max.Z) };
    }

    /**
     * Clamps a value between min and max
     */
    inline Vector4 Clamp(const Vector4 x, const Vector4 min, const Vector4 max)
    {
        return {FMath::Clamp(x.X, min.X, max.X), FMath::Clamp(x.Y, min.Y, max.Y), FMath::Clamp(x.Z, min.Z, max.Z), FMath::Clamp(x.W, min.W, max.W) };
    }

    /**
     * Clamps a value between 0 and 1
     */
    inline Vector2 Saturate(const Vector2 x)
    {
        return Clamp(x, Vector2::ZeroVector(), Vector2::OneVector());
    }

    /**
     * Clamps a value between 0 and 1
     */
    inline Vector3 Saturate(const Vector3 x)
    {
        return Clamp(x, Vector3::ZeroVector(), Vector3::OneVector());
    }

    /**
     * Clamps a value between 0 and 1
     */
    inline Vector4 Saturate(const Vector4 x)
    {
        return Clamp(x, Vector4::ZeroVector(), Vector4::OneVector());
    }

    /**
     * Clamps a value between -max and max
     */
    inline Vector2 MirrorClamp(const Vector2 x, const Vector2 max)
    {
        return Clamp(x, -max, max);
    }

    /**
     * Clamps a value between -max and max
     */
    inline Vector3 MirrorClamp(const Vector3 x, const Vector3 max)
    {
        return Clamp(x, -max, max);
    }

    /**
     * Clamps a value between -max and max
     */
    inline Vector4 MirrorClamp(const Vector4 x, const Vector4 max)
    {
        return Clamp(x, -max, max);
    }

    /**
     * Clamps a value to be above 0
     */
    inline Vector2 Max0(const Vector2 x)
    {
        return Max(x, Vector2::ZeroVector());
    }

    /**
     * Clamps a value to be above 0
     */
    inline Vector3 Max0(const Vector3 x)
    {
        return Max(x, Vector3::ZeroVector());
    }

    /**
     * Clamps a value to be above 0
     */
    inline Vector4 Max0(const Vector4 x)
    {
        return Max(x, Vector4::ZeroVector());
    }

    /**
     * Returns Max if value is above 0 and Min if value is below 0
     */
    inline Vector2 MaxOrMin(const Vector2 x, const Vector2 minValue)
    {
        return { FMath::MaxOrMin(x.X, minValue.X),
                FMath::MaxOrMin(x.Y, minValue.Y),
        };
    }

    /**
     * Returns Max if value is above 0 and Min if value is below 0
     */
    inline Vector3 MaxOrMin(const Vector3 x, const Vector3 minValue)
    {
        return { FMath::MaxOrMin(x.X, minValue.X),
                FMath::MaxOrMin(x.Y, minValue.Y),
                FMath::MaxOrMin(x.Z, minValue.Z),
        };
    }

    /**
     * Returns Max if value is above 0 and Min if value is below 0
     */
    inline Vector4 MaxOrMin(const Vector4 x, const Vector4 minValue)
    {
        return { FMath::MaxOrMin(x.X, minValue.X),
                FMath::MaxOrMin(x.Y, minValue.Y),
                FMath::MaxOrMin(x.Z, minValue.Z),
                FMath::MaxOrMin(x.W, minValue.W),
        };
    }

    /**
     * Wraps the angles in radians from 0 to 2pi
     */
    inline Vector2 WrapAngles(const Vector2 euler)
    {
        return {FMath::WrapAngle(euler.Pitch), FMath::WrapAngle(euler.Yaw)};
    }

    /**
     * Wraps the angles in radians from 0 to 2pi
     */
    inline Vector3 WrapAngles(const Vector3 euler)
    {
        return {FMath::WrapAngle(euler.Pitch), FMath::WrapAngle(euler.Yaw), FMath::WrapAngle(euler.Roll)};
    }

    /**
     * Returns the shortest angles in radians from -pi to pi
     */
    inline Vector2 DeltaAngles(const Vector2 current, const Vector2 target)
    {
        Vector2 delta = WrapAngles(target - current);
        if (delta.Pitch > FMath::PI)
            delta.Pitch -= FMath::TAU;
        else if (delta.Pitch < -FMath::PI)
            delta.Pitch += FMath::TAU;

        if (delta.Yaw > FMath::PI)
            delta.Yaw -= FMath::TAU;
        else if (delta.Yaw < -FMath::PI)
            delta.Yaw += FMath::TAU;

        return delta;
    }

    /**
     * Returns the shortest angles in radians from -pi to pi
     */
    inline Vector3 DeltaAngles(const Vector3 current, const Vector3 target)
    {
        Vector3 delta = WrapAngles(target - current);
        if (delta.Pitch > FMath::PI)
            delta.Pitch -= FMath::TAU;
        else if (delta.Pitch < -FMath::PI)
            delta.Pitch += FMath::TAU;

        if (delta.Yaw > FMath::PI)
            delta.Yaw -= FMath::TAU;
        else if (delta.Yaw < -FMath::PI)
            delta.Yaw += FMath::TAU;

        if (delta.Roll > FMath::PI)
            delta.Roll -= FMath::TAU;
        else if (delta.Roll < -FMath::PI)
            delta.Roll += FMath::TAU;

        return delta;
    }
}
