#pragma once

#include "ANE/Math/Types/Swizzle.h"
#include "TVector2.h"

namespace Engine::MathTypes
{
    template <IntegerType T>
    struct TVector<2, T>
    {
        union { T X, Width; };
        union { T Y, Height; };

        GEN_SWIZZLE_VEC2(T, X, Y)
        GEN_VECTOR2(T)

        static TVector ConvertFloor(TVector const& vec);
        static TVector ConvertRound(TVector const& vec);
        static TVector ConvertCeil(TVector const& vec);
    };

    template <IntegerType T>
    TVector<2, T> TVector<2, T>::ConvertFloor(TVector const& vec)
    {
        return {static_cast<T>(std::floor(vec.X)), static_cast<T>(std::floor(vec.Y))};
    }

    template <IntegerType T>
    TVector<2, T> TVector<2, T>::ConvertRound(TVector const& vec)
    {
        return {static_cast<T>(std::round(vec.X)), static_cast<T>(std::round(vec.Y))};
    }

    template <IntegerType T>
    TVector<2, T> TVector<2, T>::ConvertCeil(TVector const& vec)
    {
        return {static_cast<T>(std::ceil(vec.X)), static_cast<T>(std::ceil(vec.Y))};
    }
}
