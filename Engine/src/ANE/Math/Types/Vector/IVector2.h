#pragma once

#include "TVector2.h"

namespace Engine::MathTypes
{
    template <IntegerType T>
    struct TVector<2, T>
    {
        union { T X, Width; };
        union { T Y, Height; };

        GEN_VECTOR2(T)

        template <NumericType U>
        static TVector ConvertFloor(TVector<2, U> const& vec);
        template <NumericType U>
        static TVector ConvertRound(TVector<2, U> const& vec);
        template <NumericType U>
        static TVector ConvertCeil(TVector<2, U> const& vec);
    };

    template <IntegerType T>
    template <NumericType U>
    TVector<2, T> TVector<2, T>::ConvertFloor(TVector<2, U> const& vec)
    {
        return {static_cast<T>(std::floor(vec.X)), static_cast<T>(std::floor(vec.Y))};
    }

    template <IntegerType T>
    template <NumericType U>
    TVector<2, T> TVector<2, T>::ConvertRound(TVector<2, U> const& vec)
    {
        return {static_cast<T>(std::round(vec.X)), static_cast<T>(std::round(vec.Y))};
    }

    template <IntegerType T>
    template <NumericType U>
    TVector<2, T> TVector<2, T>::ConvertCeil(TVector<2, U> const& vec)
    {
        return {static_cast<T>(std::ceil(vec.X)), static_cast<T>(std::ceil(vec.Y))};
    }
}
