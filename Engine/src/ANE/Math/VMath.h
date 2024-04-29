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
}
