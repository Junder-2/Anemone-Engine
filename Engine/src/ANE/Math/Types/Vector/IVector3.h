#pragma once

#include "TVector3.h"

namespace Engine::MathTypes
{
    template <IntegerType T>
    struct TVector<3, T>
    {
        union { T X, Width; };
        union { T Y, Height; };
        union { T Z, Depth; };

        GEN_VECTOR3(T)

        template <NumericType U>
        static TVector ConvertFloor(TVector<3, U> const& vec);
        template <NumericType U>
        static TVector ConvertRound(TVector<3, U> const& vec);
        template <NumericType U>
        static TVector ConvertCeil(TVector<3, U> const& vec);
    };

    template <IntegerType T>
    template <NumericType U>
    TVector<3, T> TVector<3, T>::ConvertFloor(TVector<3, U> const& vec)
    {
        return { static_cast<T>(std::floor(vec.X)), static_cast<T>(std::floor(vec.Y), static_cast<T>(std::floor(vec.Z))) };
    }

    template <IntegerType T>
    template <NumericType U>
    TVector<3, T> TVector<3, T>::ConvertRound(TVector<3, U> const& vec)
    {
        return { static_cast<T>(std::round(vec.X)), static_cast<T>(std::round(vec.Y), static_cast<T>(std::round(vec.Z))) };
    }

    template <IntegerType T>
    template <NumericType U>
    TVector<3, T> TVector<3, T>::ConvertCeil(TVector<3, U> const& vec)
    {
        return { static_cast<T>(std::ceil(vec.X)), static_cast<T>(std::ceil(vec.Y), static_cast<T>(std::ceil(vec.Z))) };
    }
}
