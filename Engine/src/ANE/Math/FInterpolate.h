#pragma once
#include "FMath.h"
#include "MathCommon.h"

namespace Engine::FMath
{
    /**
     * Unclamped interpolate between x-y
     */
    inline float Lerp(const float x, const float y, const float t)
    {
        return x + (y - x) * t;
    }

    /**
     * Unclamped interpolate angle in radians between x-y
     */
    inline float LerpAngle(const float x, const float y, const float t)
    {
        const float delta = DeltaAngle(y - x, t);
        return x + delta * t;
    }

    /**
     * Moves towards target with speed
     */
    inline float MoveTowards(const float current, const float target, const float speed)
    {
        const float dist = target - current;
        if (Abs(dist) <= EPSILON) return target;

        return current + MirrorClamp(dist, speed);
    }

    /**
     * Moves towards target angle in radians with speed
     */
    inline float MoveTowardsAngle(const float current, const float target, const float speed)
    {
        const float dist = WrapAngle(target - current);
        if (Abs(dist) <= EPSILON) return target;

        return current + MirrorClamp(dist, speed);
    }
}
