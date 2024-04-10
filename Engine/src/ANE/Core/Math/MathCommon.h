#pragma once

namespace Engine
{
    const float EPSILON = std::numeric_limits<float>::epsilon();

    inline bool Equal(const float a, const float b, const float epsilon = EPSILON) {
        return (std::abs(a - b) < epsilon);
    }
}
