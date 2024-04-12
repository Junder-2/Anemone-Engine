#pragma once
#include "glm/gtx/fast_trigonometry.hpp"

namespace Engine
{
    namespace FMath
    {
        const float EPSILON = std::numeric_limits<float>::epsilon();

        const float TAU = glm::tau<float>();
        const float PI = glm::pi<float>();
        const float Half_PI = glm::half_pi<float>();

        const float RAD_TO_DEGREES = (180.f/PI);
        const float DEGREES_TO_RAD = (PI / 180.f);

        inline float WrapAngle(const float radians)
        {
            return glm::wrapAngle(radians);
        }

        inline bool Equal(const float a, const float b, const float epsilon = EPSILON) {
            return (std::abs(a - b) < epsilon);
        }
    }
}
