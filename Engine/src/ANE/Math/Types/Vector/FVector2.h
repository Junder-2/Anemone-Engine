#pragma once

#include "TVector2.h"

namespace Engine::MathTypes
{
    template <FloatingType T>
    struct TVector<2, T>
    {
        union { T X, Pitch, Width; };
        union { T Y, Yaw, Height; };

        GEN_VECTOR2(T)

        T Magnitude() const;
        void UnsafeNormalize();
        TVector UnsafeNormalized() const;
        void Normalize();
        TVector Normalized() const;

        static T Dot(const TVector& vec1, const TVector& vec2);
        static bool Equal(const TVector& vec1, const TVector& vec2, T epsilon = FMath::EPSILON<T>);
    };

    template <FloatingType T>
    T TVector<2, T>::Magnitude() const
    {
        return sqrt(MagnitudeSquare());
    }

    template <FloatingType T>
    void TVector<2, T>::UnsafeNormalize()
    {
        const T l = Magnitude();
        X /= l;
        Y /= l;
    }

    template <FloatingType T>
    TVector<2, T> TVector<2, T>::UnsafeNormalized() const
    {
        const T l = Magnitude();
        X /= l;
        Y /= l;
        return { X / l, Y / l };
    }

    template <FloatingType T>
    void TVector<2, T>::Normalize()
    {
        if (IsZero()) return;
        const T l = Magnitude();
        X /= l;
        Y /= l;
    }

    template <FloatingType T>
    TVector<2, T> TVector<2, T>::Normalized() const
    {
        if (IsZero()) return zeroVector;
        const T l = Magnitude();
        X /= l;
        Y /= l;
        return { X / l, Y / l };
    }

    template <FloatingType T>
    T TVector<2, T>::Dot(const TVector& vec1, const TVector& vec2)
    {
        return vec1.X * vec2.X + vec1.Y * vec2.Y;
    }

    template <FloatingType T>
    bool TVector<2, T>::Equal(const TVector& vec1, const TVector& vec2, const T epsilon)
    {
        return FMath::Equal(vec1.X, vec2.X, epsilon) && FMath::Equal(vec1.Y, vec2.Y, epsilon);
    }
}
