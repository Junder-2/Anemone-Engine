#pragma once
#include "Types/Vector2.h"
#include "Types/Vector3.h"
#include "glm/gtc/random.hpp"

namespace Engine::Random
{
    // Might want to add random with seeds
    inline float Range(const float min, const float max)
    {
        return glm::linearRand(min, max);
    }

    inline Vector2 InCircle(const float radius)
    {
        return Vector2::Convert(glm::diskRand(radius));
    }

    inline Vector2 OnCircle(const float radius)
    {
        return Vector2::Convert(glm::circularRand(radius));
    }

    inline Vector3 InSphere(const float radius)
    {
        return Vector3::Convert(glm::ballRand(radius));
    }

    inline Vector3 OnSphere(const float radius)
    {
        return Vector3::Convert(glm::sphericalRand(radius));
    }
}
