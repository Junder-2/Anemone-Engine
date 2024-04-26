#pragma once

#include "glm/gtx/compatibility.hpp"
#include "glm/gtx/fast_trigonometry.hpp"

namespace Engine::FMath
{
    /**
     * Rounds value to closest integer
     */
    inline float Round(const float x)
    {
        return glm::round(x);
    }

    /**
     * Rounds value to closest low integer
     */
    inline float Floor(const float x)
    {
        return glm::floor(x);
    }

    /**
     * Rounds value to closest high integer
     */
    inline float Ceil(const float x)
    {
        return glm::ceil(x);
    }

    /**
     * Snaps value
     */
    inline float Snap(const float x, const float scale)
    {
        return Floor(x * scale) / scale;
    }

    /**
     * Wraps an angle in radians from 0 to 2pi
     */
    inline float WrapAngle(const float radians)
    {
        return glm::wrapAngle(radians);
    }

    /**
     * Returns the sine of angle in radians
     */
    inline float Sin(const float radians)
    {
        return glm::sin(radians);
    }

    /**
     * Returns the arc sine of angle in radians
     */
    inline float Asin(const float radians)
    {
        return glm::asin(radians);
    }

    /**
     * Returns the cosine of angle in radians
     */
    inline float Cos(const float radians)
    {
        return glm::cos(radians);
    }

    /**
     * Returns the arc cosine of angle in radians
     */
    inline float Acos(const float radians)
    {
        return glm::acos(radians);
    }

    /**
     * Returns the tangent of angle in radians
     */
    inline float Tan(const float radians)
    {
        return glm::tan(radians);
    }

    /**
     * Returns the arc tangent of angle in radians
     */
    inline float Atan(const float radians)
    {
        return glm::atan(radians);
    }

    /**
     * Returns an angle in radians whose tangent is y/x
     */
    inline float Atan2(const float y, const float x)
    {
        return glm::atan2(y, x);
    }

    /**
     * Returns the smallest of the two values
     */
    inline float Min(const float x, const float y)
    {
        return glm::min(x, y);
    }

    /**
     * Returns the biggest of the two values
     */
    inline float Max(const float x, const float y)
    {
        return glm::max(x, y);
    }

    /**
     * Clamps a value between min and max
     */
    inline float Clamp(const float x, const float min, const float max)
    {
        return glm::clamp(x, min, max);
    }

    /**
     * Clamps a value between 0 and 1
     */
    inline float Saturate(const float x)
    {
        return glm::saturate(x);
    }

    /**
     * Clamps a value between -max and max
     */
    inline float MirrorClamp(const float x, const float max)
    {
        return glm::clamp(x, -max, max);
    }

    /**
     * Clamps a value to be above 0
     */
    inline float Max0(const float x)
    {
        return glm::max(x, 0.f);
    }
}
