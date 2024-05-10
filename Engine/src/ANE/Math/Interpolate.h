#pragma once
#include "FInterpolate.h"
#include "Types/Quaternion.h"
#include "Types/Vector2.h"
#include "Types/Vector3.h"
#include "Types/Vector4.h"

namespace Engine::Math
{
    /**
     * Unclamped interpolate between x-y
     */
    inline Vector2 Lerp(const Vector2 x, const Vector2 y, const float t)
    {
        return { FMath::Lerp(x.X, y.X, t), FMath::Lerp(x.Y, y.Y, t) };
    }

    /**
     * Moves towards target with speed
     */
    inline Vector2 MoveTowards(const Vector2 current, const Vector2 target, const float speed)
    {
        const Vector2 dist = target - current;
        if (dist.LengthSquare() <= FMath::EPSILON) return target;

        return current + FMath::MirrorClamp(dist.X, speed) + FMath::MirrorClamp(dist.Y, speed);
    }

    /**
     * Unclamped interpolate between x-y
     */
    inline Vector3 Lerp(const Vector3 x, const Vector3 y, const float t)
    {
        return { FMath::Lerp(x.X, y.X, t), FMath::Lerp(x.Y, y.Y, t), FMath::Lerp(x.Z, y.Z, t) };
    }

    /**
     * Moves towards target with speed
     */
    inline Vector3 MoveTowards(const Vector3 current, const Vector3 target, const float speed)
    {
        const Vector3 dist = target - current;
        if (dist.LengthSquare() <= FMath::EPSILON) return target;

        return current + FMath::MirrorClamp(dist.X, speed) + FMath::MirrorClamp(dist.Y, speed) + FMath::MirrorClamp(dist.Z, speed);
    }

    /**
     * Unclamped interpolate between x-y
     */
    inline Vector4 Lerp(const Vector4 x, const Vector4 y, const float t)
    {
        return { FMath::Lerp(x.X, y.X, t), FMath::Lerp(x.Y, y.Y, t), FMath::Lerp(x.Z, y.Z, t), FMath::Lerp(x.W, y.W, t) };
    }

    /**
     * Moves towards target with speed
     */
    inline Vector4 MoveTowards(const Vector4 current, const Vector4 target, const float speed)
    {
        const Vector4 dist = target - current;
        if (dist.LengthSquare() <= FMath::EPSILON) return target;

        return current + FMath::MirrorClamp(dist.X, speed) + FMath::MirrorClamp(dist.Y, speed) + FMath::MirrorClamp(dist.Z, speed) + FMath::MirrorClamp(dist.W, speed);
    }

    /**
     * Unclamped interpolate between x-y
     */
    inline Quaternion Lerp(const Quaternion& x, const Quaternion& y, const float t)
    {
        return Quaternion::Convert(lerp(static_cast<glm::quat>(x), static_cast<glm::quat>(y), t));
    }
}
