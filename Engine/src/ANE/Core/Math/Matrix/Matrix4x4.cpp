#include "anepch.h"
#include "Matrix4x4.h"

#include "Matrix3x3.h"
#include "ANE/Core/Math/Quaternion.h"

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
            angle *= DEGREES_TO_RAD;
        }

        glm::mat4 newMatrix = rotate(glm::mat4(*this), angle,  glm::vec3(axis));

        _rows[0] = Vector4::Convert(newMatrix[0]);
        _rows[1] = Vector4::Convert(newMatrix[1]);
        _rows[2] = Vector4::Convert(newMatrix[2]);
    }

    void Matrix4x4::Rotate(const Quaternion quat)
    {
        Matrix4x4 rotMatrix = (quat).GetMatrix();
        rotMatrix[3][3] = 1;

        *this *= rotMatrix;
    }

    void Matrix4x4::Rotate(const Vector3 euler, const bool isDegrees /*= false*/)
    {
        Rotate(euler.X, Vector3::RightVector(), isDegrees);
        Rotate(euler.Y, Vector3::UpVector(), isDegrees);
        Rotate(euler.Z, Vector3::ForwardVector(), isDegrees);
    }

    void Matrix4x4::SetRotation(const Quaternion quat)
    {
        const Vector3 scale = GetScale();

        _rows[0] = Vector4(1, 0, 0, 0) * scale.X;
        _rows[1] = Vector4(0, 1, 0, 0) * scale.Y;
        _rows[2] = Vector4(0, 0, 1, 0) * scale.Z;
        Rotate(quat);
    }

    void Matrix4x4::SetRotation(Vector3 euler, const bool isDegrees /*= false*/)
    {
        const Vector3 scale = GetScale();

        _rows[0] = Vector4(1, 0, 0, 0) * scale.X;
        _rows[1] = Vector4(0, 1, 0, 0) * scale.Y;
        _rows[2] = Vector4(0, 0, 1, 0) * scale.Z;

        if(isDegrees)
        {
            euler *= DEGREES_TO_RAD;
        }

        Rotate(euler, false);
    }

    Quaternion Matrix4x4::GetQuaternion() const
    {
        return Quaternion::Convert(quat_cast(glm::mat4(*this)));
    }

    Vector3 Matrix4x4::GetEulerAngles(bool isDegrees /*= false*/) const
    {
        return Vector3::Convert(eulerAngles(quat_cast(glm::mat4(*this)))) * (isDegrees ? RAD_TO_DEGREES : 1.f);
    }

    void Matrix4x4::Translate(const Vector3 delta)
    {
        glm::mat4 newMatrix = translate(glm::mat4(*this), glm::vec3(delta));

        _rows[3] = Vector4::Convert(newMatrix[3]);
    }

    void Matrix4x4::SetPosition(const Vector3 newPos)
    {
        const float w = _rows[3][3];
        _rows[3] = Vector4(newPos, w);
    }

    Vector3 Matrix4x4::GetPosition() const
    {
        return {_rows[3]};
    }

    void Matrix4x4::Scale(const Vector3 scale)
    {
        _rows[0] *= scale.X;
        _rows[1] *= scale.Y;
        _rows[2] *= scale.Z;
    }

    void Matrix4x4::SetScale(const Vector3 scale)
    {
        _rows[0].Normalize();
        _rows[1].Normalize();
        _rows[2].Normalize();

        _rows[0] *= scale.X;
        _rows[1] *= scale.Y;
        _rows[2] *= scale.Z;
    }

    Vector3 Matrix4x4::GetScale() const
    {
        const Vector3 scale(Vector3(_rows[0]).Length(), Vector3(_rows[1]).Length(), Vector3(_rows[2]).Length());
        return scale;
    }

    Matrix4x4 Matrix4x4::Convert(const glm::mat4& mat4)
    {
        return {Vector4::Convert(mat4[0]), Vector4::Convert(mat4[1]), Vector4::Convert(mat4[2]), Vector4::Convert(mat4[3])};
    }

    Matrix4x4::operator const glm::mat4() const
    {
        return glm::mat4(_rows[0], _rows[1], _rows[2], _rows[3]);
    }

    Matrix4x4::operator const Matrix3x3() const
    {
        return {_rows[0], _rows[1], _rows[2]};
    }

    Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& matrix)
    {
        glm::mat4 newMatrix = Convert(glm::mat4(*this) * glm::mat4(matrix));

        _rows[0] = Vector4::Convert(newMatrix[0]);
        _rows[1] = Vector4::Convert(newMatrix[1]);
        _rows[2] = Vector4::Convert(newMatrix[2]);
        _rows[3] = Vector4::Convert(newMatrix[3]);

        return *this;
    }

    Matrix4x4 operator*(const Matrix4x4& matrix1, const Matrix4x4& matrix2)
    {
        return Matrix4x4::Convert(glm::mat4(matrix1) * glm::mat4(matrix2));
    }
}
