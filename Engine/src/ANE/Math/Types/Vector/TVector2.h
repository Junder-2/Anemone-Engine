#pragma once

#include "ANE/Math/Types/MathTypes.h"
#include "ANE/Math/MathCommon.h"
#include <glm/vec2.hpp>
#include <reactphysics3d/mathematics/Vector2.h>
#include <imgui.h>

#include "ANE/Math/Types/Swizzle.h"

// General template for TVector

namespace Engine::MathTypes
{
    template <NumericType T>
    struct SwizzleAB
    {
        T X, Y;

        SwizzleAB(T a, T b) : X(a), Y(b) {}

        GEN_SWIZZLE_VEC2(T, X, Y)
        GEN_SWIZZLE_VEC2_01(T, X, Y)
    };
}

#define GEN_VECTOR2(T)                                                                                                  \
    static constexpr int Count()                                                                                        \
    {                                                                                                                   \
        return 2;                                                                                                       \
    }                                                                                                                   \
                                                                                                                        \
    std::string ToString() const                                                                                        \
    {                                                                                                                   \
        return std::format("Vector2({}, {})", X, Y);                                                                    \
    }                                                                                                                   \
                                                                                                                        \
    static std::string ToString(const TVector& vec)                                                                     \
    {                                                                                                                   \
        return vec.ToString;                                                                                            \
    }                                                                                                                   \
                                                                                                                        \
    SwizzleAB<T> Swizzle() const                                                                                        \
    {                                                                                                                   \
        return SwizzleAB<T>(X, Y);                                                                                      \
    }                                                                                                                   \
                                                                                                                        \
    static const TVector zeroVector {0, 0};                                                                             \
    static const TVector rightVector {1, 0};                                                                            \
    static const TVector upVector {0, 1};                                                                               \
    static const TVector oneVector {1, 1};                                                                              \
                                                                                                                        \
    bool IsZero() const                                                                                                 \
    {                                                                                                                   \
        return MagnitudeSquare() < FMath::EPSILON;                                                                      \
    }                                                                                                                   \
                                                                                                                        \
    bool IsNonZero() const                                                                                              \
    {                                                                                                                   \
        return !IsZero();                                                                                               \
    }                                                                                                                   \
                                                                                                                        \
    bool HasPositive() const                                                                                            \
    {                                                                                                                   \
        return (X > 0 || Y > 0);                                                                                        \
    }                                                                                                                   \
                                                                                                                        \
    bool HasNegative() const                                                                                            \
    {                                                                                                                   \
        return (X < 0 || Y < 0);                                                                                        \
    }                                                                                                                   \
                                                                                                                        \
    T MagnitudeSquare() const                                                                                           \
    {                                                                                                                   \
        return X*X + Y*Y;                                                                                               \
    }                                                                                                                   \
                                                                                                                        \
    TVector() : X(0), Y(0) {}                                                                                           \
    TVector(TVector const& v) : X(v.X), Y(v.Y) {}                                                                       \
    TVector(T scalar) : X(scalar), Y(scalar) {}                                                                         \
    TVector(T x, T y) : X(x), Y(y) {}                                                                                   \
                                                                                                                        \
    template<NumericType U>                                                                                             \
    TVector(TVector<2, U> const& v) : X(static_cast<T>(v.X)), Y(static_cast<T>(v.Y)) {}                                 \
                                                                                                                        \
    template<NumericType A, NumericType B>                                                                              \
    TVector(A x, B y) : X(static_cast<T>(x)), Y(static_cast<T>(y)) {}                                                   \
                                                                                                                        \
    /** Conversion to other vector2 types */                                                                            \
    static TVector Convert(const reactphysics3d::Vector2& vec)                                                          \
    {                                                                                                                   \
        return TVector(vec.x, vec.y);                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    static TVector Convert(const glm::vec<2, T>& vec)                                                                   \
    {                                                                                                                   \
        return TVector(vec.x, vec.y);                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    static TVector Convert(const ImVec2& vec)                                                                           \
    {                                                                                                                   \
        return TVector(vec.x, vec.y);                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    operator const reactphysics3d::Vector2() const                                                                      \
    {                                                                                                                   \
        return reactphysics3d::Vector2(X, Y);                                                                           \
    }                                                                                                                   \
                                                                                                                        \
    operator const glm::vec<2, T>() const                                                                               \
    {                                                                                                                   \
        return glm::vec<2, T>(X, Y);                                                                                    \
    }                                                                                                                   \
                                                                                                                        \
    operator const ImVec2() const                                                                                       \
    {                                                                                                                   \
        return ImVec2(ToFloat(X), ToFloat(Y));                                                                          \
    }                                                                                                                   \
                                                                                                                        \
    T& operator[](int index)                                                                                            \
    {                                                                                                                   \
        ANE_EASSERT(index < Count());                                                                                   \
        return (&X)[index];                                                                                             \
    }                                                                                                                   \
                                                                                                                        \
    T& const operator[](int index) const                                                                                \
    {                                                                                                                   \
        ANE_EASSERT(index < Count());                                                                                   \
        return (&X)[index];                                                                                             \
    }                                                                                                                   \
                                                                                                                        \
    bool operator==(const TVector& other) const                                                                         \
    {                                                                                                                   \
        return (X == other.X && Y == other.Y);                                                                          \
    }                                                                                                                   \
                                                                                                                        \
    bool operator!=(const TVector& other) const                                                                         \
    {                                                                                                                   \
        return !(*this == other);                                                                                       \
    }                                                                                                                   \
                                                                                                                        \
    bool operator<(const TVector& other) const                                                                          \
    {                                                                                                                   \
        return (X == other.X ? Y < other.Y : X < other.X);                                                              \
    }                                                                                                                   \
                                                                                                                        \
    template<NumericType U>                                                                                             \
    TVector& operator=(TVector<2, U> const& v)                                                                          \
    {                                                                                                                   \
        X = static_cast<T>(v.X);                                                                                        \
        Y = static_cast<T>(v.Y);                                                                                        \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    template<NumericType U>                                                                                             \
    TVector<2, U>& operator=(TVector const& v)                                                                          \
    {                                                                                                                   \
        X = static_cast<U>(v.X);                                                                                        \
        Y = static_cast<U>(v.Y);                                                                                        \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    TVector& operator+=(T scalar)                                                                                       \
    {                                                                                                                   \
        X += scalar;                                                                                                    \
        Y += scalar;                                                                                                    \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    TVector& operator+=(TVector const& v)                                                                               \
    {                                                                                                                   \
        X += v.X;                                                                                                       \
        Y += v.Y;                                                                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    TVector& operator-=(T scalar)                                                                                       \
    {                                                                                                                   \
        X -= scalar;                                                                                                    \
        Y -= scalar;                                                                                                    \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    TVector& operator-=(TVector const& v)                                                                               \
    {                                                                                                                   \
        X -= v.X;                                                                                                       \
        Y -= v.Y;                                                                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    TVector& operator*=(T scalar)                                                                                       \
    {                                                                                                                   \
        X *= scalar;                                                                                                    \
        Y *= scalar;                                                                                                    \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    TVector& operator*=(TVector const& v)                                                                               \
    {                                                                                                                   \
        X *= v.X;                                                                                                       \
        Y *= v.Y;                                                                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    TVector& operator/=(T scalar)                                                                                       \
    {                                                                                                                   \
        ANE_EASSERT(abs(scalar) >= FMath::EPSILON);                                                                     \
        X /= scalar;                                                                                                    \
        Y /= scalar;                                                                                                    \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    TVector& operator/=(TVector const& v)                                                                               \
    {                                                                                                                   \
        ANE_EASSERT(abs(v.X) >= FMath::EPSILON);                                                                        \
        ANE_EASSERT(abs(v.Y) >= FMath::EPSILON);                                                                        \
        X /= v.X;                                                                                                       \
        Y /= v.Y;                                                                                                       \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    friend TVector operator-(TVector const& v)                                                                          \
    {                                                                                                                   \
        return {-v.X, -v.Y};                                                                                            \
    }                                                                                                                   \
                                                                                                                        \
    friend TVector operator+(TVector const& v, T scalar)                                                                \
    {                                                                                                                   \
        return {v.X + scalar, v.Y + scalar};                                                                            \
    }                                                                                                                   \
                                                                                                                        \
    friend TVector operator+(T scalar, TVector const& v)                                                                \
    {                                                                                                                   \
        return v + scalar;                                                                                              \
    }                                                                                                                   \
                                                                                                                        \
    friend TVector operator+(TVector const& v1, TVector const& v2)                                                      \
    {                                                                                                                   \
        return {v1.X + v2.X, v1.Y + v2.Y};                                                                              \
    }                                                                                                                   \
                                                                                                                        \
    friend TVector operator-(TVector const& v, T scalar)                                                                \
    {                                                                                                                   \
        return {v.X - scalar, v.Y - scalar};                                                                            \
    }                                                                                                                   \
                                                                                                                        \
    friend TVector operator-(T scalar, TVector const& v)                                                                \
    {                                                                                                                   \
        return {scalar - v.X, scalar - v.Y};                                                                            \
    }                                                                                                                   \
                                                                                                                        \
    friend TVector operator-(TVector const& v1, TVector const& v2)                                                      \
    {                                                                                                                   \
        return {v1.X - v2.X, v1.Y - v2.Y};                                                                              \
    }                                                                                                                   \
                                                                                                                        \
    friend TVector operator*(TVector const& v, T scalar)                                                                \
    {                                                                                                                   \
        return {v.X * scalar, v.Y * scalar};                                                                            \
    }                                                                                                                   \
                                                                                                                        \
    friend TVector operator*(T scalar, TVector const& v)                                                                \
    {                                                                                                                   \
        return v * scalar;                                                                                              \
    }                                                                                                                   \
                                                                                                                        \
    friend TVector operator*(TVector const& v1, TVector const& v2)                                                      \
    {                                                                                                                   \
        return {v1.X * v2.X, v1.Y * v2.Y};                                                                              \
    }                                                                                                                   \
                                                                                                                        \
    friend TVector operator/(TVector const& v, T scalar)                                                                \
    {                                                                                                                   \
        ANE_EASSERT(abs(scalar) >= FMath::EPSILON);                                                                     \
        return v /= scalar;                                                                                             \
    }                                                                                                                   \
                                                                                                                        \
    friend TVector operator/(TVector const& v1, TVector const& v2)                                                      \
    {                                                                                                                   \
        ANE_EASSERT(abs(v1.X) >= FMath::EPSILON);                                                                       \
        ANE_EASSERT(abs(v1.Y) >= FMath::EPSILON);                                                                       \
        return {v1.X / v2.X, v1.Y / v2.Y};                                                                              \
    }