#pragma once

#include "MathCommon.h"

//Modified copy of reactphysics/include/reactphysics3d/mathematics/Vector2.h

struct ImVec2;

namespace reactphysics3d
{
    struct Vector2;
}

namespace Engine
{
    struct Vector3;
    struct Vector4;

    struct Vector2
    {
        Vector2() : X(0), Y(0) {}
        Vector2(const float scalar) : X(scalar), Y(scalar) {}
        Vector2(const float newX, const float newY) : X(newX), Y(newY) {}

        union {float X, R, Pitch;};
        union {float Y, G, Yaw;};

        static Vector2 ZeroVector() { return {0, 0}; }
        static Vector2 RightVector() { return {1, 0}; }
        static Vector2 UpVector() { return {0, 1}; }
        static Vector2 OneVector() { return {1, 1}; }

        float LengthSquare() const;

        float Length() const;

        float Dot(const Vector2& vector) const;

        bool IsZero() const;

        void Normalize();

        static bool Equal(const Vector2& vec1, const Vector2& vec2, const float epsilon = FMath::EPSILON);

        // Conversion to other vector2 types
        static Vector2 Convert(const reactphysics3d::Vector2& vec);
        static Vector2 Convert(const glm::vec2& vec);
        static Vector2 Convert(const ImVec2& vec);

        operator const reactphysics3d::Vector2() const;
        operator const glm::vec2() const;
        operator const ImVec2() const;

        operator const Vector3() const;
        operator const Vector4() const;

        bool operator==(const Vector2& vector) const
        {
            return (X == vector.X && Y == vector.Y);
        }

        bool operator!=(const Vector2& vector) const
        {
            return !(*this == vector);
        }

        Vector2& operator+=(const Vector2& vector)
        {
            X += vector.X;
            Y += vector.Y;
            return *this;
        }

        Vector2& operator-=(const Vector2& vector)
        {
            X -= vector.X;
            Y -= vector.Y;
            return *this;
        }

        Vector2& operator*=(const float& scalar)
        {
            X *= scalar;
            Y *= scalar;
            return *this;
        }

        Vector2& operator/=(const float& scalar)
        {
            assert(scalar > FMath::EPSILON);
            X /= scalar;
            Y /= scalar;
            return *this;
        }

        Vector2& operator=(const float& scalar)
        {
            X = scalar;
            Y = scalar;
            return *this;
        }

        float& operator[](const int index)
        {
            return (&X)[index];
        }

        const float& operator[](const int index) const
        {
            return (&X)[index];
        }

        bool operator<(const Vector2& vector) const
        {
            return (X == vector.X ? Y < vector.Y : X < vector.X);
        }

        friend Vector2 operator+(const Vector2& vector1, const Vector2& vector2)
        {
            return {vector1.X + vector2.X, vector1.Y + vector2.Y};
        }

        friend Vector2 operator-(const Vector2& vector1, const Vector2& vector2)
        {
            return {vector1.X - vector2.X, vector1.Y - vector2.Y};
        }

        friend Vector2 operator-(const Vector2& vector)
        {
            return {-vector.X, -vector.Y};
        }

        friend Vector2 operator*(const Vector2& vector, const float scalar)
        {
            return {scalar * vector.X, scalar * vector.Y};
        }

        friend Vector2 operator*(const float scalar, const Vector2& vector)
        {
            return {scalar * vector.X, scalar * vector.Y};
        }

        friend Vector2 operator*(const Vector2& vector1, const Vector2& vector2)
        {
            return {vector1.X*vector2.X, vector1.Y*vector2.Y};
        }

        friend Vector2 operator/(const Vector2& vector, const float scalar)
        {
            assert(scalar > FMath::EPSILON);
            return  {vector.X / scalar, vector.Y / scalar};
        }

        friend Vector2 operator/(const Vector2& vector1, const Vector2& vector2)
        {
            assert(vector2.X > FMath::EPSILON);
            assert(vector2.Y > FMath::EPSILON);
            return {vector1.X / vector2.X, vector1.Y / vector2.Y};
        }

        std::string ToString() const;
    };

    inline float Vector2::LengthSquare() const
    {
        return X*X + Y*Y;
    }

    inline float Vector2::Length() const
    {
        return std::sqrt(LengthSquare());
    }

    inline float Vector2::Dot(const Vector2& vector) const
    {
        return (X*vector.X + Y*vector.Y);
    }

    inline bool Vector2::IsZero() const
    {
        return (std::abs(LengthSquare() - 0) < FMath::EPSILON);
    }

    inline void Vector2::Normalize()
    {
        const float l = Length();
        if (l < FMath::EPSILON) {
            return;
        }
        X /= l;
        Y /= l;
    }

    inline std::string Vector2::ToString() const
    {
        return "Vector2(" + std::to_string(X) + "," + std::to_string(Y) + ")";
    }

    inline bool Vector2::Equal(const Vector2& vec1, const Vector2& vec2, const float epsilon)
    {
        return FMath::Equal(vec1.X, vec2.X, epsilon) && FMath::Equal(vec1.Y, vec2.Y, epsilon);
    }
}
