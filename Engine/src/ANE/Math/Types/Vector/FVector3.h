#pragma once

#include "TVector3.h"

namespace Engine::MathTypes
{
    template <FloatingType T>
    struct TVector<3, T>
    {
        union { T X, Pitch, Width, R; };
        union { T Y, Yaw, Height, G; };
        union { T Z, Roll, Depth, B; };

        GEN_VECTOR3(T)

        T Magnitude() const;
        void UnsafeNormalize();
        TVector UnsafeNormalized() const;
        void Normalize();
        TVector Normalized() const;

        static T Dot(const TVector& vec1, const TVector& vec2);
        static TVector Cross(const TVector& vec1, const TVector& vec2);
        static bool Equal(const TVector& vec1, const TVector& vec2, T epsilon = FMath::EPSILON<T>);
    };

    template <FloatingType T>
    T TVector<3, T>::Magnitude() const
    {
        return sqrt(MagnitudeSquare());
    }

    template <FloatingType T>
    void TVector<3, T>::UnsafeNormalize()
    {
        const float l = Magnitude();
        ANE_EASSERT(l >= FMath::EPSILON);
        X /= l;
        Y /= l;
        Z /= l;
    }

    template <FloatingType T>
    TVector<3, T> TVector<3, T>::UnsafeNormalized() const
    {
        const float l = Magnitude();
        ANE_EASSERT(l >= FMath::EPSILON);
        X /= l;
        Y /= l;
        Z /= l;
        return { X / l, Y / l, Z / l };
    }

    template <FloatingType T>
    void TVector<3, T>::Normalize()
    {
        if (IsZero()) return;
        const T l = Magnitude();
        X /= l;
        Y /= l;
        Z /= l;
    }

    template <FloatingType T>
    TVector<3, T> TVector<3, T>::Normalized() const
    {
        if (IsZero()) return zeroVector;
        const T l = Magnitude();
        X /= l;
        Y /= l;
        Z /= l;
        return { X / l, Y / l, Z / l };
    }

    template <FloatingType T>
    T TVector<3, T>::Dot(const TVector& vec1, const TVector& vec2)
    {
        return vec1.X * vec2.X + vec1.Y * vec2.Y + vec1.Z * vec2.Z;
    }

    template <FloatingType T>
    TVector<3, T> TVector<3, T>::Cross(const TVector& vec1, const TVector& vec2)
    {
        return {vec1.Y * vec2.Z - vec1.Z * vec2.Y, vec1.Z * vec2.X - vec1.X * vec2.Z, vec1.X * vec2.Y - vec1.Y * vec2.X};
    }

    template <FloatingType T>
    bool TVector<3, T>::Equal(const TVector& vec1, const TVector& vec2, T epsilon)
    {
        return FMath::Equal(vec1.X, vec2.X, epsilon) && FMath::Equal(vec1.Y, vec2.Y, epsilon) && FMath::Equal(vec1.Z, vec2.Z, epsilon);
    }
}
