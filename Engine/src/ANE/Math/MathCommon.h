#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/constants.hpp"

namespace Engine::FMath
{
    template<FloatingType T = float>
    const T EPSILON = std::numeric_limits<T>::epsilon();

    template<FloatingType T = float>
    const T TAU = glm::tau<T>();
    template<FloatingType T = float>
    const T PI = glm::pi<T>();
    template<FloatingType T = float>
    const T HALF_PI = glm::half_pi<float>();

    template<FloatingType T = float>
    const T RAD_TO_DEGREES = (T(180)/PI<T>);
    template<FloatingType T = float>
    const T DEGREES_TO_RAD = (PI<T> / T(180));

    template<FloatingType T = float>
    bool Equal(const T a, const T b, const T epsilon = EPSILON<T>)
    {
        return (std::abs(a - b) < epsilon);
    }
}
