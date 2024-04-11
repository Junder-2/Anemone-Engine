#include "anepch.h"
#include "Matrix4x4.h"

#include "Matrix3x3.h"

namespace Engine
{
    Matrix4x4 Matrix4x4::GetTranspose() const
    {
        return Convert(transpose(static_cast<glm::mat4>(*this)));
    }

    float Matrix4x4::GetDeterminant() const
    {
        return determinant(static_cast<glm::mat4>(*this));
    }

    Matrix4x4 Matrix4x4::GetInverse() const
    {
        return Convert(inverse(static_cast<glm::mat4>(*this)));
    }

    void Matrix4x4::Translate(Vector3 delta)
    {
        glm::mat4 newMatrix = translate(glm::mat4(*this), glm::vec3(delta));

        _rows[3] = Vector4::Convert(newMatrix[3]);
    }

    void Matrix4x4::Rotate(const float angle, const Vector3 axis)
    {
        glm::mat4 newMatrix = rotate(glm::mat4(*this), angle, glm::vec3(axis));

        _rows[0] = Vector4::Convert(newMatrix[0]);
        _rows[1] = Vector4::Convert(newMatrix[1]);
        _rows[2] = Vector4::Convert(newMatrix[2]);
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
        glm::mat4 newMatrix = Convert(static_cast<glm::mat4>(*this) * static_cast<glm::mat4>(matrix));

        _rows[0] = Vector4::Convert(newMatrix[0]);
        _rows[1] = Vector4::Convert(newMatrix[1]);
        _rows[2] = Vector4::Convert(newMatrix[2]);
        _rows[3] = Vector4::Convert(newMatrix[3]);

        return *this;
    }

    Matrix4x4 operator*(const Matrix4x4& matrix1, const Matrix4x4& matrix2)
    {
        return Matrix4x4::Convert(static_cast<glm::mat4>(matrix1) * static_cast<glm::mat4>(matrix2));
    }
}
