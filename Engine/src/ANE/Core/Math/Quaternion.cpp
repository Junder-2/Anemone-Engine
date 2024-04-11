#include "anepch.h"
#include "Quaternion.h"

#include <reactphysics3d/mathematics/Quaternion.h>

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
        return glm::quat(X, Y, Z, W);
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
}
