#include "anepch.h"
#include "Matrix3x3.h"

#include <reactphysics3d/mathematics/Matrix3x3.h>
#include "Matrix4x4.h"

namespace Engine
{
    Matrix3x3 Matrix3x3::GetTranspose() const
    {
        return Convert(transpose(static_cast<glm::mat3>(*this)));
    }

    float Matrix3x3::GetDeterminant() const
    {
        return determinant(static_cast<glm::mat3>(*this));
    }

    Matrix3x3 Matrix3x3::GetInverse() const
    {
        return Convert(inverse(static_cast<glm::mat3>(*this)));
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
        glm::mat3 newMatrix = Convert(static_cast<glm::mat3>(*this) * static_cast<glm::mat3>(matrix));

        _rows[0] = Vector3::Convert(newMatrix[0]);
        _rows[1] = Vector3::Convert(newMatrix[1]);
        _rows[2] = Vector3::Convert(newMatrix[2]);

        return *this;
    }

    Matrix3x3 operator*(const Matrix3x3& matrix1, const Matrix3x3& matrix2)
    {
        return Matrix3x3::Convert(static_cast<glm::mat3>(matrix1) * static_cast<glm::mat3>(matrix2));
    }
}
