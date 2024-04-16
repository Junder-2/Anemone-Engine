#include "anepch.h"
#include "Matrix3x3.h"

#include <reactphysics3d/mathematics/Matrix3x3.h>
#include "Matrix4x4.h"
#include "ANE/Core/Math/Quaternion.h"
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

        _rows[0] = Vector4::Convert(newMatrix[0]);
        _rows[1] = Vector4::Convert(newMatrix[1]);
        _rows[2] = Vector4::Convert(newMatrix[2]);
    }

    void Matrix3x3::Rotate(const Vector3 euler, const bool isDegrees /*= false*/)
    {
        Rotate(euler.Yaw, Vector3::UpVector(), isDegrees);
        Rotate(euler.Pitch, Vector3::RightVector(), isDegrees);
        Rotate(euler.Roll, Vector3::ForwardVector(), isDegrees);
    }

    void Matrix3x3::SetRotation(const Quaternion quat)
    {
        const Vector3 scale = GetScale();

        _rows[0] = Vector4(1, 0, 0, 0) * scale.X;
        _rows[1] = Vector4(0, 1, 0, 0) * scale.Y;
        _rows[2] = Vector4(0, 0, 1, 0) * scale.Z;
        Rotate(quat);
    }

    void Matrix3x3::SetRotation(Vector3 euler, const bool isDegrees /*= false*/)
    {
        const Vector3 scale = GetScale();

        _rows[0] = Vector4(1, 0, 0, 0) * scale.X;
        _rows[1] = Vector4(0, 1, 0, 0) * scale.Y;
        _rows[2] = Vector4(0, 0, 1, 0) * scale.Z;

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

        euler.Yaw = glm::asin(-copy[0][2]);
        if (glm::cos(euler.Yaw) != 0) {
            euler.Pitch = glm::atan2(copy[1][2], copy[2][2]);
            euler.Roll = glm::atan2(copy[0][1], copy[0][0]);
        }
        else {
            euler.Pitch = glm::atan2(-copy[2][0], copy[1][1]);
            euler.Roll = 0;
        }

        return euler * (isDegrees ? FMath::RAD_TO_DEGREES : 1);
        //return Vector3::Convert(eulerAngles(quat_cast(glm::mat4(*this)))) * (isDegrees ? RAD_TO_DEGREES : 1.f);
    }

    void Matrix3x3::SetScale(const Vector3 scale)
    {
        _rows[0].Normalize();
        _rows[1].Normalize();
        _rows[2].Normalize();

        _rows[0] *= scale.X;
        _rows[1] *= scale.Y;
        _rows[2] *= scale.Z;
    }

    void Matrix3x3::Scale(const Vector3 scale)
    {
        _rows[0] *= scale.X;
        _rows[1] *= scale.Y;
        _rows[2] *= scale.Z;
    }

    Vector3 Matrix3x3::GetScale() const
    {
        const Vector3 scale(Vector3(_rows[0]).Length(), Vector3(_rows[1]).Length(), Vector3(_rows[2]).Length());
        return scale;
    }

    Vector3 Matrix3x3::GetRight() const
    {
        return _rows[0];
    }

    Vector3 Matrix3x3::GetUp() const
    {
        return _rows[1];
    }

    Vector3 Matrix3x3::GetForward() const
    {
        return _rows[2];
    }

    Matrix3x3 Matrix3x3::Convert(const glm::mat3& mat3)
    {
        return {Vector3::Convert(mat3[0]), Vector3::Convert(mat3[1]), Vector3::Convert(mat3[2])};
    }

    Matrix3x3::operator const reactphysics3d::Matrix3x3() const
    {
        return reactphysics3d::Matrix3x3(
        _rows[0][0], _rows[0][1], _rows[0][2],
        _rows[1][0], _rows[1][1], _rows[1][2],
        _rows[2][0], _rows[2][1], _rows[2][2]);
    }

    Matrix3x3::operator const glm::mat3() const
    {
        return glm::mat3(_rows[0], _rows[1], _rows[2]);
    }

    Matrix3x3::operator const Matrix4x4() const
    {
        return {Vector4(_rows[0]), Vector4(_rows[1]), Vector4(_rows[2]), Vector4::ZeroVector()};
    }

    Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& matrix)
    {
        glm::mat3 newMatrix = Convert(glm::mat3(*this) * glm::mat3(matrix));

        _rows[0] = Vector3::Convert(newMatrix[0]);
        _rows[1] = Vector3::Convert(newMatrix[1]);
        _rows[2] = Vector3::Convert(newMatrix[2]);

        return *this;
    }

    Matrix3x3 operator*(const Matrix3x3& matrix1, const Matrix3x3& matrix2)
    {
        return Matrix3x3::Convert(glm::mat3(matrix1) * glm::mat3(matrix2));
    }
}
