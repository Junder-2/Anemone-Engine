#include "anepch.h"
#include "Matrix3x3.h"

#include <reactphysics3d/mathematics/Matrix3x3.h>
#include "Matrix4x4.h"
#include "ANE/Math/FMath.h"
#include "ANE/Math/Types/Quaternion.h"
#include "ANE/Physics/PhysicsTypes.h"
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

    Matrix4x4 Matrix3x3::GetNormalized() const
    {
        auto copy = Matrix3x3(*this);
        copy[0].Normalize();
        copy[1].Normalize();
        copy[2].Normalize();

        return copy;
    }

    void Matrix3x3::SetRotation(const Quaternion& quat)
    {
        const Vector3 scale = GetScale();

        _columns[0] = Vector4(1, 0, 0, 0);
        _columns[1] = Vector4(0, 1, 0, 0);
        _columns[2] = Vector4(0, 0, 1, 0);
        Rotate(quat);
        if(Vector3::Equal(scale, Vector3::OneVector())) return;
        Scale(scale);
    }

    void Matrix3x3::Rotate(const Quaternion& quat)
    {
        const Matrix3x3 rotMatrix = (quat).GetMatrix();
        *this *= rotMatrix;
    }

    Quaternion Matrix3x3::GetQuaternionFast() const
    {
        return Quaternion::Convert(quat_cast(glm::mat3(this->GetNormalized())));
    }

    Quaternion Matrix3x3::GetQuaternion() const
    {
        // glm::decompose()

        Matrix3x3 copy = this->GetNormalized();
        if(Vector3::Dot(copy[0], Vector3::Cross(copy[1], copy[2])) < 0)
        {
            copy[0] *= -1.f;
            copy[1] *= -1.f;
            copy[2] *= -1.f;
        }

        Quaternion orientation;
        float root;
        if(const float trace = copy[0].X + copy[1].Y + copy[2].Z; trace > 0.f)
        {
            root = sqrt(trace + 1.f);
            orientation.W = 0.5f * root;
            root = 0.5f / root;
            orientation.X = root * (copy[1].Z - copy[2].Y);
            orientation.Y = root * (copy[2].X - copy[0].Z);
            orientation.Z = root * (copy[0].Y - copy[1].X);
        }
        else
        {
            const int next[3] = {1, 2, 0};
            int i = 0;
            if(copy[1].Y > copy[0].X) i = 1;
            if(copy[2].Z > copy[i][i]) i = 2;
            const int j = next[i];
            const int k = next[j];

            root = sqrt(copy[i][i] - copy[j][j] - copy[k][k] + 1.f);

            orientation[i] = 0.5f * root;
            root = 0.5f / root;
            orientation[j] = root * (copy[i][j] + copy[j][i]);
            orientation[k] = root * (copy[i][k] + copy[k][i]);
            orientation.W = root * (copy[j][k] - copy[k][j]);
        }
        return orientation;
    }

    void Matrix3x3::SetRotation(Vector3 euler, const bool isDegrees /*= false*/)
    {
        const Vector3 scale = GetScaleFast();

        _columns[0] = Vector4(1, 0, 0, 0);
        _columns[1] = Vector4(0, 1, 0, 0);
        _columns[2] = Vector4(0, 0, 1, 0);

        if(isDegrees)
        {
            euler *= FMath::DEGREES_TO_RAD;
        }

        Rotate(euler, false);
        if(Vector3::Equal(scale, Vector3::OneVector())) return;
        Scale(scale);
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

    Vector3 Matrix3x3::GetEulerAngles(const bool isDegrees /*= false*/) const
    {
        Vector3 euler;
        extractEulerAngleYXZ(glm::mat4x4(*this), euler.Yaw, euler.Pitch, euler.Roll);

        return euler * (isDegrees ? FMath::RAD_TO_DEGREES : 1.f);
    }

    void Matrix3x3::SetScale(const Vector3 scale)
    {
        _columns[0].Normalize();
        _columns[1].Normalize();
        _columns[2].Normalize();

        if(Vector3::Equal(scale, Vector3::OneVector())) return;
        Scale(scale);
    }

    void Matrix3x3::Scale(const Vector3 scale)
    {
        _columns[0] *= FMath::MaxOrMin(scale.X, FMath::EPSILON);
        _columns[1] *= FMath::MaxOrMin(scale.Y, FMath::EPSILON);
        _columns[2] *= FMath::MaxOrMin(scale.Z, FMath::EPSILON);
    }

    Vector3 Matrix3x3::GetScaleFast() const
    {
        const Vector3 scale(Vector3(_columns[0]).Length(), Vector3(_columns[1]).Length(), Vector3(_columns[2]).Length());
        return scale;
    }

    Vector3 Matrix3x3::GetScale() const
    {
        Matrix3x3 copy = this->GetNormalized();
        Vector3 scale = GetScaleFast();

        const Vector3 pDum3 = Vector3::Cross(copy[1], copy[2]);
        if(Vector3::Dot(copy[0], pDum3) < 0)
        {
            scale.X *= -1.f;
            scale.Y *= -1.f;
            scale.Z *= -1.f;
        }

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

    Matrix3x3 Matrix3x3::Convert(const reactphysics3d::Matrix3x3& mat3)
    {
        return {Vector3::Convert(mat3[0]), Vector3::Convert(mat3[1]), Vector3::Convert(mat3[2])};
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
