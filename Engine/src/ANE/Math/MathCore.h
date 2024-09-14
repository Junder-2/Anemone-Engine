#pragma once

namespace Engine
{
    template<typename T>
    concept NumericType = std::is_arithmetic_v<T>;

    template<typename T>
    concept IntegerType = std::is_integral_v<T>;

    template<typename T>
    concept FloatingType = std::is_floating_point_v<T>;
}

