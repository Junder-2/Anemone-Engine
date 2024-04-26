//Modified copy of reactphysics/include/reactphysics3d/mathematics/Quaternion.h

#pragma once

#include "Vector3.h"

namespace reactphysics3d
{
    struct Quaternion;
}

namespace Engine
{
    struct Matrix3x3;

    struct Quaternion
    {
        Quaternion() : X(0), Y(0), Z(0), W(0) {}

        Quaternion(const float newX, const float newY, const float newZ, const float newW) : X(newX), Y(newY), Z(newZ), W(newW) {}

        Quaternion(const Vector3& v, const float newW) : X(v.X), Y(v.Y), Z(v.Z), W(newW) {}

        Quaternion(const Vector3& eulerAngles);

        Quaternion(const Matrix3x3& matrix);

        float X, Y, Z, W;

        static Quaternion Identity() { return {0, 0, 0, 1}; }

        Vector3 GetVectorV() const;

        float LengthSquare() const;

        float Length() const;

        void Normalize();

        float Dot(const Quaternion& quaternion) const;

        void Inverse();

        Quaternion GetNormalized() const;
        Quaternion GetConjugate() const;
        Quaternion GetInverse() const;

        Matrix3x3 GetMatrix() const;
        Vector3 GetEulerAngles(bool inDegrees = false) const;

        std::string ToString() const;

        static Quaternion FromEulerAngles(const Vector3& eulerAngles, bool inDegrees = false);

        // Conversion to other quaternion types
        static Quaternion Convert(const reactphysics3d::Quaternion& quat);
        static Quaternion Convert(const glm::quat& quat);

        operator const reactphysics3d::Quaternion() const;
        operator const glm::quat() const;

        bool operator==(const Quaternion& quaternion) const
        {
            return (X == quaternion.X && Y == quaternion.Y && Z == quaternion.Z && W == quaternion.W);
        }

        bool operator!=(const Quaternion& quaternion) const
        {
            return !(*this == quaternion);
        }

        Quaternion& operator=(const Quaternion& quaternion)
        {
            X = quaternion.X;
            Y = quaternion.Y;
            Z = quaternion.Z;
            W = quaternion.W;
            return *this;
        }

        Quaternion& operator+=(const Quaternion& quaternion)
        {
            X += quaternion.X;
            Y += quaternion.Y;
            Z += quaternion.Z;
            W += quaternion.W;
            return *this;
        }

        Quaternion& operator-=(const Quaternion& quaternion)
        {
            X -= quaternion.X;
            Y -= quaternion.Y;
            Z -= quaternion.Z;
            W -= quaternion.W;
            return *this;
        }

        Quaternion& operator*=(const Quaternion& quaternion);
        Quaternion& operator*=(float scalar);

        Quaternion operator+(const Quaternion& quaternion) const
        {
            return {X + quaternion.X, Y + quaternion.Y, Z + quaternion.Z, W + quaternion.W};
        }

        Quaternion operator-(const Quaternion& quaternion) const
        {
            return {X - quaternion.X, Y - quaternion.Y, Z - quaternion.Z, W - quaternion.W};
        }

        Quaternion operator-() const
        {
            return {-X, -Y, -Z, -W};
        }

        Quaternion operator*(const float scalar) const
        {
            return {scalar * X, scalar * Y, scalar * Z, scalar * W};
        }

        Quaternion operator*(const Quaternion& quaternion) const;
        Vector3 operator*(const Vector3& point) const;
        Vector4 operator*(const Vector4& point) const;

        friend Quaternion operator*(const float scalar, const Quaternion& quaternion)
        {
            return quaternion * scalar;
        }

        friend Vector3 operator*(const Vector3& point, const Quaternion& quaternion);
        friend Vector4 operator*(const Vector4& point, const Quaternion& quaternion);

    private:
        void InitWithEulerAngles(float pitch, float yaw, float roll);
    };

    inline Vector3 Quaternion::GetVectorV() const
    {
        return {X, Y, Z};
    }

    inline float Quaternion::LengthSquare() const
    {
        return X*X + Y*Y + Z*Z + W*W;
    }

    inline float Quaternion::Length() const
    {
        return std::sqrt(LengthSquare());
    }

    inline void Quaternion::Normalize()
    {
        const float l = Length();

        // Check if the length is not equal to zero
        ANE_EASSERT(l > FMath::EPSILON);

        X /= l;
        Y /= l;
        Z /= l;
        W /= l;
    }

    inline Quaternion Quaternion::GetNormalized() const
    {
        const float l = Length();

        // Check if the length is not equal to zero
        ANE_EASSERT(l > FMath::EPSILON);

        return {X/l, Y/l, Z/l, W/l};
    }

    inline void Quaternion::Inverse()
    {
        // Use the conjugate of the current quaternion
        X = -X;
        Y = -Y;
        Z = -Z;
    }

    inline Quaternion Quaternion::GetConjugate() const
    {
        return {-X, -Y, -Z, W};
    }

    inline Quaternion Quaternion::GetInverse() const
    {
        return {-X, -Y, -Z, W};
    }

    inline float Quaternion::Dot(const Quaternion& quaternion) const
    {
        return (X*quaternion.X + Y*quaternion.Y + Z*quaternion.Z + W*quaternion.W);
    }

    inline std::string Quaternion::ToString() const
    {
        return "Quaternion(" + std::to_string(X) + "," + std::to_string(Y) + "," + std::to_string(Z) + "," + std::to_string(W) + ")";
    }

}
