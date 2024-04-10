#pragma once
#include "Matrix3x3.h"
#include "Vector3.h"
#include "Matrix/Matrix3x3.h"

//Modified copy of reactphysics/include/reactphysics3d/mathematics/Quaternion.h

namespace reactphysics3d
{
    struct Quaternion;
}

namespace Engine
{
    struct Quaternion
    {
        Quaternion() : X(0), Y(0), Z(0), W(0) {}

        Quaternion(const float newX, const float newY, const float newZ, const float newW) : X(newX), Y(newY), Z(newZ), W(newW) {}

        Quaternion(const Vector3& v, const float newW) : X(v.X), Y(v.Y), Z(v.Z), W(newW) {}

        Quaternion(const Matrix3x3& matrix);

        float X, Y, Z, W;

        Vector3 GetVectorV() const;

        float LengthSquare() const;

        float Length() const;

        void Normalize();

        float Dot(const Quaternion& quaternion) const;

        void Inverse();

        Quaternion GetConjugate() const;

        Quaternion GetInverse() const;

        Matrix3x3 GetMatrix() const;

        std::string ToString() const;

        static Quaternion Identity();

        static Quaternion FromEulerAngles(float angleX, float angleY, float angleZ);

        static Quaternion FromEulerAngles(const Vector3& eulerAngles);

        // Conversion to other vector3 types
        operator const reactphysics3d::Quaternion&() const;
        operator const glm::quat&() const;

        Quaternion operator+(const Quaternion& quaternion) const
        {
            return {X + quaternion.X, Y + quaternion.Y, Z + quaternion.Z, W + quaternion.W};
        }

        Quaternion operator-(const Quaternion& quaternion) const
        {
            return {X - quaternion.X, Y - quaternion.Y, Z - quaternion.Z, W - quaternion.W};
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

        Quaternion operator*(const float nb) const
        {
            return {nb * X, nb * Y, nb * Z, nb * W};
        }

        Quaternion operator*(const Quaternion& quaternion) const
        {
            return Quaternion(W * quaternion.X + quaternion.W * X + Y * quaternion.Z - Z * quaternion.Y,
                      W * quaternion.Y + quaternion.W * Y + Z * quaternion.X - X * quaternion.Z,
                      W * quaternion.Z + quaternion.W * Z + X * quaternion.Y - Y * quaternion.X,
                      W * quaternion.W - X * quaternion.X - Y * quaternion.Y - Z * quaternion.Z);
        }

        Vector3 operator*(const Vector3& point) const
        {
            const float prodX = W * point.X + Y * point.Z - Z * point.Y;
            const float prodY = W * point.Y + Z * point.X - X * point.Z;
            const float prodZ = W * point.Z + X * point.Y - Y * point.X;
            const float prodW = -X * point.X - Y * point.Y - Z * point.Z;

            return      {W * prodX - prodY * Z + prodZ * Y - prodW * X,
                        W * prodY - prodZ * X + prodX * Z - prodW * Y,
                        W * prodZ - prodX * Y + prodY * X - prodW * Z};
        }

        bool operator==(const Quaternion& quaternion) const
        {
            return (X == quaternion.X && Y == quaternion.Y &&
            Z == quaternion.Z && W == quaternion.W);
        }

    private:
        void InitWithEulerAngles(float angleX, float angleY, float angleZ);
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
        assert (l > EPSILON);

        X /= l;
        Y /= l;
        Z /= l;
        W /= l;
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

    inline Quaternion Quaternion::Identity()
    {
        return Quaternion(0.0, 0.0, 0.0, 1.0);
    }

    inline std::string Quaternion::ToString() const
    {
        return "Quaternion(" + std::to_string(X) + "," + std::to_string(Y) + "," + std::to_string(Z) + "," + std::to_string(W) + ")";
    }

}
