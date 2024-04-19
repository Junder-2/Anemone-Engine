#pragma once

#include "MathCommon.h"

//Modified copy of reactphysics/include/reactphysics3d/mathematics/Vector3.h

namespace reactphysics3d
{
    struct Vector3;
}

namespace Engine
{
    struct Vector2;
    struct Vector4;

    struct Vector3
    {
        Vector3() : X(0), Y(0), Z(0) {}
        Vector3(const float scalar) : X(scalar), Y(scalar), Z(scalar) {}
        Vector3(const float newX, const float newY, const float newZ) : X(newX), Y(newY), Z(newZ) {}
        Vector3(Vector2 vector2, float newZ = 0);

        union {float X, R, Pitch;};
        union {float Y, G, Yaw;};
        union {float Z, B, Roll;};

        static Vector3 ZeroVector() { return {0, 0, 0}; }
        static Vector3 RightVector() { return {1, 0, 0}; }
        static Vector3 UpVector() { return {0, 1, 0}; }
        static Vector3 ForwardVector() { return {0, 0, 1}; }
        static Vector3 OneVector() { return {1, 1, 0}; }

        float LengthSquare() const;

        float Length() const;

        float Dot(const Vector3& vector) const;

        Vector3 Cross(const Vector3& vector) const;

        bool IsZero() const;

        void Normalize();

        Vector3 GetNormalized() const;

        static bool Equal(const Vector3& vec1, const Vector3& vec2, const float epsilon = FMath::EPSILON);

        static Vector3 Convert(const reactphysics3d::Vector3& vec);
        static Vector3 Convert(const glm::vec3& vec);

        // Conversion to other vector3 types
        operator const reactphysics3d::Vector3() const;
        operator const glm::vec3() const;

        operator const Vector2() const;
        operator const Vector4() const;

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
            assert(scalar > FMath::EPSILON);
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

        friend Vector3 operator+(const Vector3& vector1, const Vector3& vector2)
        {
            return {vector1.X + vector2.X, vector1.Y + vector2.Y, vector1.Z + vector2.Z};
        }

        friend Vector3 operator-(const Vector3& vector1, const Vector3& vector2)
        {
            return {vector1.X - vector2.X, vector1.Y - vector2.Y, vector1.Z - vector2.Z};
        }

        friend Vector3 operator-(const Vector3& vector)
        {
            return {-vector.X, -vector.Y, -vector.Z};
        }

        friend Vector3 operator*(const Vector3& vector, const float scalar)
        {
            return {scalar * vector.X, scalar * vector.Y, scalar * vector.Z};
        }

        friend Vector3 operator*(const float scalar, const Vector3& vector)
        {
            return vector * scalar;
        }

        friend Vector3 operator*(const Vector3& vector1, const Vector3& vector2)
        {
            return {vector1.X*vector2.X, vector1.Y*vector2.Y, vector1.Z*vector2.Z};
        }

        friend Vector3 operator/(const Vector3& vector, const float scalar)
        {
            assert(scalar > FMath::EPSILON);
            return {vector.X / scalar, vector.Y / scalar, vector.Z / scalar};
        }

        friend Vector3 operator/(const Vector3& vector1, const Vector3& vector2)
        {
            assert(vector2.X > FMath::EPSILON);
            assert(vector2.Y > FMath::EPSILON);
            assert(vector2.Z > FMath::EPSILON);
            return {vector1.X / vector2.X, vector1.Y / vector2.Y, vector1.Z / vector2.Z};
        }

        std::string ToString() const;
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
        return (std::abs(LengthSquare() - 0) < FMath::EPSILON);
    }

    inline Vector3 Vector3::GetNormalized() const
    {
        Vector3 copy = *this;
        const float l = Length();
        if (l < FMath::EPSILON) {
            return ZeroVector();
        }
        copy.X /= l;
        copy.Y /= l;
        copy.Z /= l;

        return copy;
    }

    inline void Vector3::Normalize()
    {
        *this = this->GetNormalized();
    }

    inline std::string Vector3::ToString() const
    {
        return "Vector3(" + std::to_string(X) + "," + std::to_string(Y) + "," + std::to_string(Z) + ")";
    }

    inline bool Vector3::Equal(const Vector3& vec1, const Vector3& vec2, const float epsilon)
    {
        return FMath::Equal(vec1.X, vec2.X, epsilon) && FMath::Equal(vec1.Y, vec2.Y, epsilon) &&
            FMath::Equal(vec1.Z, vec2.Z, epsilon);
    }
}
