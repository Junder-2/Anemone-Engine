#include "anepch.h"
#include "Matrix4x4.h"

#include "Matrix3x3.h"
#include "ANE/Core/Math/Quaternion.h"
#include "glm/gtx/compatibility.hpp"

namespace Engine
{
    Matrix4x4 Matrix4x4::GetTranspose() const
    {
        return Convert(transpose(glm::mat4(*this)));
    }

    float Matrix4x4::GetDeterminant() const
    {
        return determinant(glm::mat4(*this));
    }

    Matrix4x4 Matrix4x4::GetInverse() const
    {
        return Convert(inverse(glm::mat4(*this)));
    }

    void Matrix4x4::Rotate(float angle, const Vector3 axis, const bool isDegrees /*= false*/)
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

    void Matrix4x4::Rotate(const Quaternion quat)
    {
        Matrix4x4 rotMatrix = (quat).GetMatrix();
        rotMatrix[3][3] = 1;

        *this *= rotMatrix;
    }

    void Matrix4x4::Rotate(const Vector3 euler, const bool isDegrees /*= false*/)
    {
        Rotate(FMath::WrapAngle(euler.Yaw), Vector3::UpVector(), isDegrees);
        Rotate(FMath::WrapAngle(euler.Pitch), Vector3::RightVector(), isDegrees);
        Rotate(FMath::WrapAngle(euler.Roll), Vector3::ForwardVector(), isDegrees);
    }

    void Matrix4x4::SetRotation(const Quaternion quat)
    {
        const Vector3 scale = GetScale();

        _columns[0] = Vector4(1, 0, 0, 0) * scale.X;
        _columns[1] = Vector4(0, 1, 0, 0) * scale.Y;
        _columns[2] = Vector4(0, 0, 1, 0) * scale.Z;
        Rotate(quat);
    }

    void Matrix4x4::SetRotation(Vector3 euler, const bool isDegrees /*= false*/)
    {
        const Vector3 scale = GetScale();

        _columns[0] = Vector4(1, 0, 0, 0) * scale.X;
        _columns[1] = Vector4(0, 1, 0, 0) * scale.Y;
        _columns[2] = Vector4(0, 0, 1, 0) * scale.Z;

        if(isDegrees)
        {
            euler *= FMath::DEGREES_TO_RAD;
        }

        Rotate(euler, false);
    }

    Quaternion Matrix4x4::GetQuaternion() const
    {
        return Quaternion::Convert(quat_cast(glm::mat4(*this)));
    }

    Vector3 Matrix4x4::GetEulerAngles(bool isDegrees /*= false*/) const
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

    void Matrix4x4::Translate(const Vector3 delta)
    {
        glm::mat4 newMatrix = translate(glm::mat4(*this), glm::vec3(delta));

        _columns[3] = Vector4::Convert(newMatrix[3]);
    }

    void Matrix4x4::AddPosition(const Vector3 delta)
    {
        _columns[3] += Vector4(delta, 0);
    }

    void Matrix4x4::SetPosition(const Vector3 newPos)
    {
        const float w = _columns[3][3];
        _columns[3] = Vector4(newPos, w);
    }

    Vector3 Matrix4x4::GetPosition() const
    {
        return {_columns[3]};
    }

    void Matrix4x4::Scale(const Vector3 scale)
    {
        _columns[0] *= scale.X;
        _columns[1] *= scale.Y;
        _columns[2] *= scale.Z;
    }

    void Matrix4x4::SetScale(const Vector3 scale)
    {
        _columns[0].Normalize();
        _columns[1].Normalize();
        _columns[2].Normalize();

        _columns[0] *= scale.X;
        _columns[1] *= scale.Y;
        _columns[2] *= scale.Z;
    }

    Vector3 Matrix4x4::GetScale() const
    {
        const Vector3 scale(Vector3(_columns[0]).Length(), Vector3(_columns[1]).Length(), Vector3(_columns[2]).Length());
        return scale;
    }

    Vector3 Matrix4x4::GetRight() const
    {
        return _columns[0];
    }

    Vector3 Matrix4x4::GetUp() const
    {
        return _columns[1];
    }

    Vector3 Matrix4x4::GetForward() const
    {
        return _columns[2];
    }

    Matrix4x4 Matrix4x4::Convert(const glm::mat4& mat4)
    {
        return {Vector4::Convert(mat4[0]), Vector4::Convert(mat4[1]), Vector4::Convert(mat4[2]), Vector4::Convert(mat4[3])};
    }

    Matrix4x4::operator const glm::mat4() const
    {
        return glm::mat4(_columns[0], _columns[1], _columns[2], _columns[3]);
    }

    Matrix4x4::operator const Matrix3x3() const
    {
        return {_columns[0], _columns[1], _columns[2]};
    }

    Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& matrix)
    {
        glm::mat4 newMatrix = Convert(glm::mat4(*this) * glm::mat4(matrix));

        _columns[0] = Vector4::Convert(newMatrix[0]);
        _columns[1] = Vector4::Convert(newMatrix[1]);
        _columns[2] = Vector4::Convert(newMatrix[2]);
        _columns[3] = Vector4::Convert(newMatrix[3]);

        return *this;
    }

    Matrix4x4 operator*(const Matrix4x4& matrix1, const Matrix4x4& matrix2)
    {
        return Matrix4x4::Convert(glm::mat4(matrix1) * glm::mat4(matrix2));
    }
}
