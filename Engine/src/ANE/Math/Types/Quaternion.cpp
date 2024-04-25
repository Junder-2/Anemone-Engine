#include "anepch.h"
#include "Quaternion.h"

#include <reactphysics3d/mathematics/Quaternion.h>

#include "Matrix3x3.h"
#include "Vector4.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Engine
{
    Quaternion Quaternion::Convert(const reactphysics3d::Quaternion& quat)
    {
        return {quat.x, quat.y, quat.z, quat.w};
    }

    Quaternion Quaternion::Convert(const glm::quat& quat)
    {
        return {quat.x, quat.y, quat.z, quat.w};
    }

    Quaternion::operator const reactphysics3d::Quaternion() const
    {
        return reactphysics3d::Quaternion(X, Y, Z, W);
    }

    Quaternion::operator const glm::quat() const
    {
        return glm::quat(W, X, Y, Z);
    }

    Quaternion::Quaternion(const Vector3& eulerAngles)
    {
        InitWithEulerAngles(eulerAngles.Pitch, eulerAngles.Yaw, eulerAngles.Roll);
    }

    Quaternion::Quaternion(const Matrix3x3& matrix)
    {
        const glm::qua<float> newQuat = quat_cast(glm::mat3(matrix));

        X = newQuat.x;
        Y = newQuat.y;
        Z = newQuat.z;
        W = newQuat.w;
    }

    Matrix3x3 Quaternion::GetMatrix() const
    {
        return Matrix3x3::Convert(mat3_cast(glm::quat(*this)));
    }

    Vector3 Quaternion::GetEulerAngles() const
    {
        return Vector3::Convert(eulerAngles(glm::quat(*this)));
    }

    Quaternion Quaternion::FromEulerAngles(float angleX, float angleY, float angleZ)
    {
        Quaternion quaternion;
        quaternion.InitWithEulerAngles(angleX, angleY, angleZ);

        return quaternion;
    }

    Quaternion Quaternion::FromEulerAngles(const Vector3& eulerAngles)
    {
        Quaternion quaternion;
        quaternion.InitWithEulerAngles(eulerAngles.X, eulerAngles.Y, eulerAngles.Z);

        return quaternion;
    }

    void Quaternion::InitWithEulerAngles(const float pitch, const float yaw, const float roll)
    {
        const glm::quat newQuat = glm::quat(glm::vec3(pitch, yaw, roll));

        X = newQuat.x;
        Y = newQuat.y;
        Z = newQuat.z;
        W = newQuat.w;
    }

    Quaternion& Quaternion::operator*=(const Quaternion& quaternion)
    {
        *this = *this * quaternion;
        return *this;
    }

    Quaternion& Quaternion::operator*=(const float scalar)
    {
        *this = *this * scalar;
        return *this;
    }

    Quaternion Quaternion::operator*(const Quaternion& quaternion) const
    {
        return Convert(glm::quat(*this) * glm::quat(quaternion));
    }

    Vector3 Quaternion::operator*(const Vector3& point) const
    {
        return Vector3::Convert(glm::quat(*this) * glm::vec3(point));
    }

    Vector4 Quaternion::operator*(const Vector4& point) const
    {
        return Vector4::Convert(glm::quat(*this) * glm::vec4(point));
    }

    Vector3 operator*(const Vector3& point, const Quaternion& quaternion)
    {
        return quaternion.GetInverse() * point;
    }

    Vector4 operator*(const Vector4& point, const Quaternion& quaternion)
    {
        return quaternion.GetInverse() * point;
    }
}
