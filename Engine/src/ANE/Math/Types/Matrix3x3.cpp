#include "anepch.h"
#include "Matrix3x3.h"

#include <reactphysics3d/mathematics/Matrix3x3.h>
#include "Matrix4x4.h"
#include "ANE/Math/FMath.h"
#include "ANE/Math/Types/Quaternion.h"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/matrix_decompose.hpp"

namespace Engine
{
    Matrix3x3 Matrix3x3::GetTranspose() const
    {
        return Convert(transpose(glm::mat3(*this)));
    }

    float Matrix3x3::GetDeterminant() const
    {
        return determinant(glm::mat3(*this));
    }

    Matrix3x3 Matrix3x3::GetInverse() const
    {
        return Convert(inverse(glm::mat3(*this)));
    }

    void Matrix3x3::Rotate(const Quaternion quat)
    {
        const Matrix3x3 rotMatrix = (quat).GetMatrix();
        *this *= rotMatrix;
    }

    void Matrix3x3::Rotate(float angle, const Vector3 axis, const bool isDegrees /*= false*/)
    {
        if(isDegrees)
        {
            angle *= FMath::DEGREES_TO_RAD;
        }

        glm::mat4 newMatrix = rotate(glm::mat4(*this), angle,  glm::vec3(axis));

        _columns[0] = Vector4::Convert(newMatrix[0]);
        _columns[1] = Vector4::Convert(newMatrix[1]);
        _columns[2] = Vector4::Convert(newMatrix[2]);
    }

    void Matrix3x3::Rotate(const Vector3 euler, const bool isDegrees /*= false*/)
    {
        Rotate(FMath::WrapAngle(euler.Yaw), Vector3::UpVector(), isDegrees);
        Rotate(FMath::WrapAngle(euler.Pitch), Vector3::RightVector(), isDegrees);
        Rotate(FMath::WrapAngle(euler.Roll), Vector3::ForwardVector(), isDegrees);
    }

    void Matrix3x3::SetRotation(const Quaternion quat)
    {
        const Vector3 scale = GetScale();

        _columns[0] = Vector4(1, 0, 0, 0) * scale.X;
        _columns[1] = Vector4(0, 1, 0, 0) * scale.Y;
        _columns[2] = Vector4(0, 0, 1, 0) * scale.Z;
        Rotate(quat);
    }

    void Matrix3x3::SetRotation(Vector3 euler, const bool isDegrees /*= false*/)
    {
        const Vector3 scale = GetScale();

        _columns[0] = Vector4(1, 0, 0, 0) * scale.X;
        _columns[1] = Vector4(0, 1, 0, 0) * scale.Y;
        _columns[2] = Vector4(0, 0, 1, 0) * scale.Z;

        if(isDegrees)
        {
            euler *= FMath::DEGREES_TO_RAD;
        }

        Rotate(Quaternion(euler));
    }

    Quaternion Matrix3x3::GetQuaternion() const
    {
        return Quaternion::Convert(quat_cast(glm::mat3(*this)));
    }

    Vector3 Matrix3x3::GetEulerAngles(const bool isDegrees /*= false*/) const
    {
        Matrix3x3 copy = *this;

        copy[0].Normalize();
        copy[1].Normalize();
        copy[2].Normalize();

        Vector3 euler;

        euler.Yaw = FMath::Asin(-copy[0][2]);
        if (FMath::Cos(euler.Yaw) != 0) {
            euler.Pitch = FMath::Atan2(copy[1][2], copy[2][2]);
            euler.Roll = FMath::Atan2(copy[0][1], copy[0][0]);
        }
        else {
            euler.Pitch = FMath::Atan2(-copy[2][0], copy[1][1]);
            euler.Roll = 0;
        }

        return euler * (isDegrees ? FMath::RAD_TO_DEGREES : 1);
        //return Vector3::Convert(eulerAngles(quat_cast(glm::mat4(*this)))) * (isDegrees ? RAD_TO_DEGREES : 1.f);
    }

    void Matrix3x3::SetScale(const Vector3 scale)
    {
        _columns[0].Normalize();
        _columns[1].Normalize();
        _columns[2].Normalize();

        _columns[0] *= scale.X;
        _columns[1] *= scale.Y;
        _columns[2] *= scale.Z;
    }

    void Matrix3x3::Scale(const Vector3 scale)
    {
        _columns[0] *= scale.X;
        _columns[1] *= scale.Y;
        _columns[2] *= scale.Z;
    }

    Vector3 Matrix3x3::GetScale() const
    {
        const Vector3 scale(Vector3(_columns[0]).Length(), Vector3(_columns[1]).Length(), Vector3(_columns[2]).Length());
        return scale;
    }

    Vector3 Matrix3x3::GetRight() const
    {
        return _columns[0].GetNormalized();
    }

    Vector3 Matrix3x3::GetUp() const
    {
        return _columns[1].GetNormalized();
    }

    Vector3 Matrix3x3::GetForward() const
    {
        return _columns[2].GetNormalized();
    }

    Matrix3x3 Matrix3x3::Convert(const glm::mat3& mat3)
    {
        return {Vector3::Convert(mat3[0]), Vector3::Convert(mat3[1]), Vector3::Convert(mat3[2])};
    }

    Matrix3x3::operator const reactphysics3d::Matrix3x3() const
    {
        return reactphysics3d::Matrix3x3(
        _columns[0][0], _columns[0][1], _columns[0][2],
        _columns[1][0], _columns[1][1], _columns[1][2],
        _columns[2][0], _columns[2][1], _columns[2][2]);
    }

    Matrix3x3::operator const glm::mat3() const
    {
        return glm::mat3(_columns[0], _columns[1], _columns[2]);
    }

    Matrix3x3::operator const Matrix4x4() const
    {
        return {Vector4(_columns[0]), Vector4(_columns[1]), Vector4(_columns[2]), Vector4::ZeroVector()};
    }

    Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& matrix)
    {
        glm::mat3 newMatrix = Convert(glm::mat3(*this) * glm::mat3(matrix));

        _columns[0] = Vector3::Convert(newMatrix[0]);
        _columns[1] = Vector3::Convert(newMatrix[1]);
        _columns[2] = Vector3::Convert(newMatrix[2]);

        return *this;
    }

    Matrix3x3 operator*(const Matrix3x3& matrix1, const Matrix3x3& matrix2)
    {
        return Matrix3x3::Convert(glm::mat3(matrix1) * glm::mat3(matrix2));
    }
}
