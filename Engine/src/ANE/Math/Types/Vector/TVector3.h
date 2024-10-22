#pragma once

#include "ANE/Math/Types/MathTypes.h"
#include "ANE/Math/MathCommon.h"
#include <glm/vec3.hpp>
#include <reactphysics3d/mathematics/Vector3.h>

#include "ANE/Math/Types/Swizzle.h"

// General template for TVector

namespace Engine::MathTypes
{

    template <NumericType T>
    struct SwizzleABC
    {
        T X, Y, Z;

        SwizzleABC(T a, T b, T c) : X(a), Y(b), Z(c) {}

        GEN_SWIZZLE_VEC3(T, X, Y, Z)
        GEN_SWIZZLE_VEC3_01(T, X, Y, Z)
    };
}

#define GEN_VECTOR3(T)                                                                                              \
    static constexpr int Count()                                                                                    \
    {                                                                                                               \
        return 3;                                                                                                   \
    }                                                                                                               \
                                                                                                                    \
    std::string ToString() const                                                                                    \
    {                                                                                                               \
        return std::format("Vector3({}, {}, {})", X, Y, Z);                                                         \
    }                                                                                                               \
                                                                                                                    \
    static std::string ToString(const TVector& vec)                                                                 \
    {                                                                                                               \
        return vec.ToString;                                                                                        \
    }                                                                                                               \
                                                                                                                    \
    SwizzleABC<T> Swizzle() const                                                                                   \
    {                                                                                                               \
        return SwizzleABC<T>(X, Y, Z);                                                                              \
    }                                                                                                               \
                                                                                                                    \
    static const TVector zeroVector {0, 0, 0};                                                                      \
    static const TVector rightVector {1, 0, 0};                                                                     \
    static const TVector upVector {0, 1, 0};                                                                        \
    static const TVector forwardVector {0, 0, 1};                                                                   \
    static const TVector oneVector {1, 1, 1};                                                                       \
                                                                                                                    \
    bool IsZero() const                                                                                             \
    {                                                                                                               \
        return MagnitudeSquare() < FMath::EPSILON;                                                                  \
    }                                                                                                               \
                                                                                                                    \
    bool IsNonZero() const                                                                                          \
    {                                                                                                               \
        return !IsZero();                                                                                           \
    }                                                                                                               \
                                                                                                                    \
    bool HasPositive() const                                                                                        \
    {                                                                                                               \
        return (X > 0 || Y > 0 || Z > 0);                                                                           \
    }                                                                                                               \
                                                                                                                    \
    bool HasNegative() const                                                                                        \
    {                                                                                                               \
        return (X < 0 || Y < 0 || Z < 0);                                                                           \
    }                                                                                                               \
                                                                                                                    \
    T MagnitudeSquare() const                                                                                       \
    {                                                                                                               \
        return X*X + Y*Y + Z*Z;                                                                                     \
    }                                                                                                               \
                                                                                                                    \
    TVector() : X(0), Y(0), Z(0) {}                                                                                 \
    TVector(TVector const& v) : X(v.X), Y(v.Y), Z(v.Z) {}                                                           \
    TVector(T scalar) : X(scalar), Y(scalar), Z(scalar) {}                                                          \
    TVector(T x, T y, T z) : X(x), Y(y), Z(z) {}                                                                    \
                                                                                                                    \
    template<NumericType A>                                                                                         \
    TVector(TVector<3, A> const& v) : X(static_cast<T>(v.X)), Y(static_cast<T>(v.Y)), Z(static_cast<T>(v.Z)) {}     \
                                                                                                                    \
    template<NumericType A, NumericType B, NumericType C>                                                           \
    TVector(A x, B y, C z) : X(static_cast<T>(x)), Y(static_cast<T>(y)), Z(static_cast<T>(z)) {}                    \
                                                                                                                    \
    /** Conversion to other vector3 types */                                                                        \
    static TVector Convert(const reactphysics3d::Vector3& vec)                                                      \
    {                                                                                                               \
        return TVector(vec.x, vec.y, vec.z);                                                                        \
    }                                                                                                               \
                                                                                                                    \
    static TVector Convert(const glm::vec<3, T>& vec)                                                               \
    {                                                                                                               \
        return TVector(vec.x, vec.y, vec.z);                                                                        \
    }                                                                                                               \
                                                                                                                    \
    operator const reactphysics3d::Vector3() const                                                                  \
    {                                                                                                               \
        return reactphysics3d::Vector3(X, Y, Z);                                                                    \
    }                                                                                                               \
                                                                                                                    \
    operator const glm::vec<3, T>() const                                                                           \
    {                                                                                                               \
        return glm::vec<3, T>(X, Y, Z);                                                                             \
    }                                                                                                               \
                                                                                                                    \
    template<NumericType U>                                                                                         \
    operator const TVector<3, U>() const                                                                            \
    {                                                                                                               \
        return {static_cast<U>(X), static_cast<U>(Y), static_cast<U>(Z)};                                           \
    }                                                                                                               \
                                                                                                                    \
    T& operator[](int index)                                                                                        \
    {                                                                                                               \
        ANE_EASSERT(index < Count());                                                                               \
        return (&X)[index];                                                                                         \
    }                                                                                                               \
                                                                                                                    \
    T& const operator[](int index) const                                                                            \
    {                                                                                                               \
        ANE_EASSERT(index < Count());                                                                               \
        return (&X)[index];                                                                                         \
    }                                                                                                               \
                                                                                                                    \
    bool operator==(const TVector& other) const                                                                     \
    {                                                                                                               \
        return (X == other.X && Y == other.Y && Z == other.Z);                                                      \
    }                                                                                                               \
                                                                                                                    \
    bool operator!=(const TVector& other) const                                                                     \
    {                                                                                                               \
        return !(*this == other);                                                                                   \
    }                                                                                                               \
                                                                                                                    \
    bool operator<(const TVector& other) const                                                                      \
    {                                                                                                               \
        return (X == other.X ? (Y == other.Y ? Z < other.Z : Y < other.Y) : X < other.X);                           \
    }                                                                                                               \
                                                                                                                    \
    template<NumericType U>                                                                                         \
    TVector& operator=(TVector<3, U> const& v)                                                                      \
    {                                                                                                               \
        X = static_cast<T>(v.X);                                                                                    \
        Y = static_cast<T>(v.Y);                                                                                    \
        Z = static_cast<T>(v.Z);                                                                                    \
        return *this;                                                                                               \
    }                                                                                                               \
                                                                                                                    \
    template<NumericType U>                                                                                         \
    TVector<3, U>& operator=(TVector const& v)                                                                      \
    {                                                                                                               \
        X = static_cast<U>(v.X);                                                                                    \
        Y = static_cast<U>(v.Y);                                                                                    \
        Z = static_cast<U>(v.Z);                                                                                    \
        return *this;                                                                                               \
    }                                                                                                               \
                                                                                                                    \
    TVector& operator+=(T scalar)                                                                                   \
    {                                                                                                               \
        X += scalar;                                                                                                \
        Y += scalar;                                                                                                \
        Z += scalar;                                                                                                \
        return *this;                                                                                               \
    }                                                                                                               \
                                                                                                                    \
    template<NumericType U>                                                                                         \
    TVector& operator+=(TVector<3, U> const& v)                                                                     \
    {                                                                                                               \
        X += static_cast<T>(v.X);                                                                                   \
        Y += static_cast<T>(v.Y);                                                                                   \
        Z += static_cast<T>(v.Z);                                                                                   \
        return *this;                                                                                               \
    }                                                                                                               \
                                                                                                                    \
    TVector& operator-=(T scalar)                                                                                   \
    {                                                                                                               \
        X -= scalar;                                                                                                \
        Y -= scalar;                                                                                                \
        Z -= scalar;                                                                                                \
        return *this;                                                                                               \
    }                                                                                                               \
                                                                                                                    \
    template<NumericType U>                                                                                         \
    TVector& operator-=(TVector<3, U> const& v)                                                                     \
    {                                                                                                               \
        X -= static_cast<T>(v.X);                                                                                   \
        Y -= static_cast<T>(v.Y);                                                                                   \
        Z -= static_cast<T>(v.Z);                                                                                   \
        return *this;                                                                                               \
    }                                                                                                               \
                                                                                                                    \
    TVector& operator*=(T scalar)                                                                                   \
    {                                                                                                               \
        X *= scalar;                                                                                                \
        Y *= scalar;                                                                                                \
        Z *= scalar;                                                                                                \
        return *this;                                                                                               \
    }                                                                                                               \
                                                                                                                    \
    template<NumericType U>                                                                                         \
    TVector& operator*=(TVector<3, U> const& v)                                                                     \
    {                                                                                                               \
        X *= static_cast<T>(v.X);                                                                                   \
        Y *= static_cast<T>(v.Y);                                                                                   \
        Z *= static_cast<T>(v.Z);                                                                                   \
        return *this;                                                                                               \
    }                                                                                                               \
                                                                                                                    \
    TVector& operator/=(T scalar)                                                                                   \
    {                                                                                                               \
        ANE_EASSERT(abs(scalar) >= FMath::EPSILON);                                                                 \
        X /= scalar;                                                                                                \
        Y /= scalar;                                                                                                \
        Z /= scalar;                                                                                                \
        return *this;                                                                                               \
    }                                                                                                               \
                                                                                                                    \
    template<NumericType U>                                                                                         \
    TVector& operator/=(TVector<3, U> const& v)                                                                     \
    {                                                                                                               \
        ANE_EASSERT(abs(v.X) >= FMath::EPSILON);                                                                    \
        ANE_EASSERT(abs(v.Y) >= FMath::EPSILON);                                                                    \
        ANE_EASSERT(abs(v.Z) >= FMath::EPSILON);                                                                    \
        X /= static_cast<T>(v.X);                                                                                   \
        Y /= static_cast<T>(v.Y);                                                                                   \
        Z /= static_cast<T>(v.Z);                                                                                   \
        return *this;                                                                                               \
    }                                                                                                               \
                                                                                                                    \
    template<NumericType U>                                                                                         \
    friend TVector operator-(TVector<3, U> const& v)                                                                \
    {                                                                                                               \
        return {-v.X, -v.Y, -v.Z};                                                                                  \
    }                                                                                                               \
                                                                                                                    \
    friend TVector operator+(TVector const& v, T scalar)                                                            \
    {                                                                                                               \
        return {v.X + scalar, v.Y + scalar, v.Z + scalar};                                                          \
    }                                                                                                               \
                                                                                                                    \
    friend TVector operator+(T scalar, TVector const& v)                                                            \
    {                                                                                                               \
        return v + scalar;                                                                                          \
    }                                                                                                               \
                                                                                                                    \
    template<NumericType U>                                                                                         \
    friend TVector operator+(TVector const& v1, TVector<3, U> const& v2)                                            \
    {                                                                                                               \
        return {v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z};                                                             \
    }                                                                                                               \
                                                                                                                    \
    friend TVector operator-(TVector const& v, T scalar)                                                            \
    {                                                                                                               \
        return {v.X - scalar, v.Y - scalar, v.Z - scalar};                                                          \
    }                                                                                                               \
                                                                                                                    \
    friend TVector operator-(T scalar, TVector const& v)                                                            \
    {                                                                                                               \
        return {scalar - v.X, scalar - v.Y, scalar - v.Z};                                                          \
    }                                                                                                               \
                                                                                                                    \
    template<NumericType U>                                                                                         \
    friend TVector operator-(TVector const& v1, TVector<3, U> const& v2)                                            \
    {                                                                                                               \
        return {v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z};                                                             \
    }                                                                                                               \
                                                                                                                    \
    friend TVector operator*(TVector const& v, T scalar)                                                            \
    {                                                                                                               \
        return {v.X * scalar, v.Y * scalar, v.Z * scalar};                                                          \
    }                                                                                                               \
                                                                                                                    \
    friend TVector operator*(T scalar, TVector const& v)                                                            \
    {                                                                                                               \
        return v * scalar;                                                                                          \
    }                                                                                                               \
                                                                                                                    \
    template<NumericType U>                                                                                         \
    friend TVector operator*(TVector const& v1, TVector<3, U> const& v2)                                            \
    {                                                                                                               \
        return {v1.X * v2.X, v1.Y * v2.Y, v1.Z * v2.Z};                                                             \
    }                                                                                                               \
    friend TVector operator/(TVector const& v, T scalar)                                                            \
    {                                                                                                               \
        ANE_EASSERT(abs(scalar) >= FMath::EPSILON);                                                                 \
        return v /= scalar;                                                                                         \
    }                                                                                                               \
                                                                                                                    \
    template<NumericType U>                                                                                         \
    friend TVector operator/(TVector const& v1, TVector<3, U> const& v2)                                            \
    {                                                                                                               \
        ANE_EASSERT(abs(v1.X) >= FMath::EPSILON);                                                                   \
        ANE_EASSERT(abs(v1.Y) >= FMath::EPSILON);                                                                   \
        ANE_EASSERT(abs(v1.Z) >= FMath::EPSILON);                                                                   \
        return {v1.X / v2.X, v1.Y / v2.Y, v1.Z / v2.Z};                                                             \
    }                                                                                                               \
