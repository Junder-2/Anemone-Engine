#pragma once
#include "MathCommon.h"

//Modified copy of reactphysics/include/reactphysics3d/mathematics/Vector3.h

namespace Engine
{
    struct Vector2;
    struct Vector3;

    struct Vector4
    {
        Vector4() : X(0), Y(0), Z(0), W(0) {}
        Vector4(const float newX, const float newY = 0, const float newZ = 0, const float newW = 0) : X(newX), Y(newY), Z(newZ), W(newW) {}
        Vector4(Vector2 vector2, float newZ = 0, float newW = 0);
        Vector4(Vector3 vector3, float newW = 0);

        union {float X, R;};
        union {float Y, G;};
        union {float Z, B;};
        union {float W, A;};

        inline static Vector4 ZeroVector = {0, 0, 0, 0};
        inline static Vector4 OneVector = {1, 1, 1, 1};

        float LengthSquare() const;

        float Length() const;

        float Dot(const Vector4& vector) const;

        bool IsZero() const;

        void Normalize();

        static bool Equal(const Vector4& vec1, const Vector4& vec2, float epsilon = EPSILON);

        static Vector4 Convert(const glm::vec4& vec);

        // Conversion to other vector4 types
        operator const glm::vec4() const;

        operator const Vector2() const;
        operator const Vector3() const;

        bool operator==(const Vector4& vector) const
        {
            return (X == vector.X && Y == vector.Y && Z == vector.Z && W == vector.W);
        }

        bool operator!=(const Vector4& vector) const
        {
            return !(*this == vector);
        }

        Vector4& operator+=(const Vector4& vector)
        {
            X += vector.X;
            Y += vector.Y;
            Z += vector.Z;
            W += vector.W;
            return *this;
        }

        Vector4& operator-=(const Vector4& vector)
        {
            X -= vector.X;
            Y -= vector.Y;
            Z -= vector.Z;
            W -= vector.W;
            return *this;
        }

        Vector4& operator*=(const float& scalar)
        {
            X *= scalar;
            Y *= scalar;
            Z *= scalar;
            W *= scalar;
            return *this;
        }

        Vector4& operator/=(const float& scalar)
        {
            assert(scalar > EPSILON);
            X /= scalar;
            Y /= scalar;
            Z /= scalar;
            W /= scalar;
            return *this;
        }

        Vector4& operator=(const float& scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
            W = scalar;
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

        bool operator<(const Vector4& vector) const
        {
            //TODO:
            return (X == vector.X ? (Y == vector.Y ? Z < vector.Z : Y < vector.Y) : X < vector.X);
        }

        friend Vector4 operator+(const Vector4& vector1, const Vector4& vector2)
        {
            return {vector1.X + vector2.X, vector1.Y + vector2.Y, vector1.Z + vector2.Z, vector1.W + vector2.W};
        }

        friend Vector4 operator-(const Vector4& vector1, const Vector4& vector2)
        {
            return {vector1.X - vector2.X, vector1.Y - vector2.Y, vector1.Z - vector2.Z, vector1.W - vector2.W};
        }

        friend Vector4 operator-(const Vector4& vector)
        {
            return {-vector.X, -vector.Y, -vector.Z, -vector.W};
        }

        friend Vector4 operator*(const Vector4& vector, const float scalar)
        {
            return {scalar * vector.X, scalar * vector.Y, scalar * vector.Z, scalar * vector.W};
        }

        friend Vector4 operator*(const float scalar, const Vector4& vector)
        {
            return vector * scalar;
        }

        friend Vector4 operator*(const Vector4& vector1, const Vector4& vector2)
        {
            return {vector1.X*vector2.X, vector1.Y*vector2.Y, vector1.Z*vector2.Z, vector1.W*vector2.W};
        }

        friend Vector4 operator/(const Vector4& vector, const float scalar)
        {
            assert(scalar > EPSILON);
            return {vector.X / scalar, vector.Y / scalar, vector.Z / scalar, vector.W / scalar};
        }

        friend Vector4 operator/(const Vector4& vector1, const Vector4& vector2)
        {
            assert(vector2.X > EPSILON);
            assert(vector2.Y > EPSILON);
            assert(vector2.Z > EPSILON);
            assert(vector2.W > EPSILON);
            return {vector1.X / vector2.X, vector1.Y / vector2.Y, vector1.Z / vector2.Z, vector1.W / vector2.W};
        }

        std::string ToString() const;
    };

    inline float Vector4::LengthSquare() const
    {
        return X*X + Y*Y + Z*Z + W*W;
    }

    inline float Vector4::Length() const
    {
        return std::sqrt(LengthSquare());
    }

    inline float Vector4::Dot(const Vector4& vector) const
    {
        return (X*vector.X + Y*vector.Y + Z*vector.Z + W*vector.W);
    }

    inline bool Vector4::IsZero() const
    {
        return (std::abs(LengthSquare() - 0) < EPSILON);
    }

    inline void Vector4::Normalize()
    {
        const float l = Length();
        if (l < EPSILON) {
            return;
        }
        X /= l;
        Y /= l;
        Z /= l;
        W /= l;
    }

    inline std::string Vector4::ToString() const
    {
        return "Vector4(" + std::to_string(X) + "," + std::to_string(Y) + "," + std::to_string(Z) + "," + std::to_string(W) + ")";
    }

    inline bool Vector4::Equal(const Vector4& vec1, const Vector4& vec2, const float epsilon)
    {
        return Engine::Equal(vec1.X, vec2.X, epsilon) && Engine::Equal(vec1.Y, vec2.Y, epsilon) &&
            Engine::Equal(vec1.Z, vec2.Z, epsilon) && Engine::Equal(vec1.W, vec2.W, epsilon);
    }
}
