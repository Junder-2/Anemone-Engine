#pragma once

#include "MathCommon.h"

//Modified copy of reactphysics/include/reactphysics3d/mathematics/Vector3.h

namespace reactphysics3d
{
    struct Vector3;
}

namespace Engine
{
    struct Vector3
    {
        Vector3(const float newX = 0, const float newY = 0, const float newZ = 0) : X(newX), Y(newY), Z(newZ) {}

        float X, Y, Z;

        float LengthSquare() const;

        float Length() const;

        float Dot(const Vector3& vector) const;

        Vector3 Cross(const Vector3& vector) const;

        bool IsZero() const;

        void Normalize();

        std::string ToString() const;

        static Vector3 ZeroVector();

        static bool Equal(const Vector3& vec1, const Vector3& vec2, const float epsilon = EPSILON);

        // Conversion to other vector3 types
        operator const reactphysics3d::Vector3&() const;
        operator const glm::vec3&() const;

        bool operator==(const Vector3& vector) const
        {
            return (X == vector.X && Y == vector.Y && Z == vector.Z);
        }

        bool operator!=(const Vector3& vector) const
        {
            return !(*this == vector);
        }

        Vector3& operator+=(const Vector3& vector)
        {
            X += vector.X;
            Y += vector.Y;
            Z += vector.Z;
            return *this;
        }

        Vector3& operator-=(const Vector3& vector)
        {
            X -= vector.X;
            Y -= vector.Y;
            Z -= vector.Z;
            return *this;
        }

        Vector3& operator*=(const float& scalar)
        {
            X *= scalar;
            Y *= scalar;
            Z *= scalar;
            return *this;
        }

        Vector3& operator/=(const float& scalar)
        {
            assert(scalar > EPSILON);
            X /= scalar;
            Y /= scalar;
            Z /= scalar;
            return *this;
        }

        Vector3& operator=(const float& scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
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

        bool operator<(const Vector3& vector) const
        {
            return (X == vector.X ? (Y == vector.Y ? Z < vector.Z : Y < vector.Y) : X < vector.X);
        }

        Vector3 operator+(const Vector3& vector1, const Vector3& vector2)
        {
            return {vector1.X + vector2.X, vector1.Y + vector2.Y, vector1.Z + vector2.Z};
        }

        Vector3 operator-(const Vector3& vector1, const Vector3& vector2)
        {
            return {vector1.X - vector2.X, vector1.Y - vector2.Y, vector1.Z - vector2.Z};
        }

        Vector3 operator-(const Vector3& vector)
        {
            return {-vector.X, -vector.Y, -vector.Z};
        }

        Vector3 operator*(const Vector3& vector, const float scalar)
        {
            return {scalar * vector.X, scalar * vector.Y, scalar * vector.Z};
        }

        Vector3 operator*(const float scalar, const Vector3& vector)
        {
            return {scalar * vector.X, scalar * vector.Y, scalar * vector.Z};
        }

        Vector3 operator*(const Vector3& vector1, const Vector3& vector2)
        {
            return {vector1.X*vector2.X, vector1.Y*vector2.Y, vector1.Z*vector2.Z};
        }

        Vector3 operator/(const Vector3& vector, const float scalar)
        {
            assert(scalar > EPSILON);
            return {vector.X / scalar, vector.Y / scalar, vector.Z / scalar};
        }

        Vector3 operator/(const Vector3& vector1, const Vector3& vector2)
        {
            assert(vector2.X > EPSILON);
            assert(vector2.Y > EPSILON);
            assert(vector2.Z > EPSILON);
            return {vector1.X / vector2.X, vector1.Y / vector2.Y, vector1.Z / vector2.Z};
        }
    };

    inline float Vector3::LengthSquare() const
    {
        return X*X + Y*Y + Z*Z;
    }

    inline float Vector3::Length() const
    {
        return std::sqrt(LengthSquare());
    }

    inline float Vector3::Dot(const Vector3& vector) const
    {
        return (X*vector.X + Y*vector.Y + Z*vector.Z);
    }

    inline Vector3 Vector3::Cross(const Vector3& vector) const
    {
        return {Y * vector.Z - Z * vector.Y,
                       Z * vector.X - X * vector.Z,
                       X * vector.Y - Y * vector.X};
    }

    inline bool Vector3::IsZero() const
    {
        return (std::abs(LengthSquare() - 0) < EPSILON);
    }

    inline void Vector3::Normalize()
    {
        const float l = Length();
        if (l < EPSILON) {
            return;
        }
        X /= l;
        Y /= l;
        Z /= l;
    }

    inline std::string Vector3::ToString() const
    {
        return "Vector3(" + std::to_string(X) + "," + std::to_string(Y) + "," + std::to_string(Z) + ")";
    }

    inline Vector3 Vector3::ZeroVector()
    {
        return {0, 0, 0};
    }

    inline bool Vector3::Equal(const Vector3& vec1, const Vector3& vec2, const float epsilon)
    {
        return Equal(vec1.X, vec2.X, epsilon) && Equal(vec1.Y, vec2.Y, epsilon) &&
            Equal(vec1.Z, vec2.Z, epsilon);
    }
}
