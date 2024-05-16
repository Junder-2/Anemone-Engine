#include "anepch.h"
#include "Matrix4x4.h"

#include "Matrix3x3.h"
#include "ANE/Math/Types/Quaternion.h"
#include "ANE/Math/FMath.h"
#include "ANE/Math/VMath.h"
#include "ANE/Physics/PhysicsTypes.h"
#include "glm/gtx/euler_angles.hpp"

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

    Matrix4x4 Matrix4x4::GetNormalized() const
    {
        auto copy = Matrix4x4(*this);
        copy[0].Normalize();
        copy[1].Normalize();
        copy[2].Normalize();

        return copy;
    }

    void Matrix4x4::SetRotation(const Quaternion& quat)
    {
        const Vector3 scale = GetScale();

        _columns[0] = Vector4(1, 0, 0, 0);
        _columns[1] = Vector4(0, 1, 0, 0);
        _columns[2] = Vector4(0, 0, 1, 0);
        Rotate(quat);
        if(Vector3::Equal(scale, Vector3::OneVector())) return;
        Scale(scale);
    }

    void Matrix4x4::Rotate(const Quaternion& quat)
    {
        Matrix4x4 rotMatrix = (quat).GetMatrix();
        rotMatrix[3][3] = 1;

        *this *= rotMatrix;
    }

    Quaternion Matrix4x4::GetQuaternionFast() const
    {
        auto quat = Quaternion::Convert(quat_cast(glm::mat3(this->GetNormalized())));
        return quat;
    }

    Quaternion Matrix4x4::GetQuaternion() const
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

    void Matrix4x4::SetRotation(Vector3 euler, const bool isDegrees /*= false*/)
    {
        const Vector3 scale = GetScale();

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

    void Matrix4x4::Rotate(const Vector3 euler, const bool isDegrees /*= false*/)
    {
        Rotate(euler.Yaw, Vector3::UpVector(), isDegrees);
        Rotate(euler.Pitch, Vector3::RightVector(), isDegrees);
        Rotate(euler.Roll, Vector3::ForwardVector(), isDegrees);
    }

    Vector3 Matrix4x4::GetEulerAngles(const bool isDegrees /*= false*/) const
    {
        Vector3 euler;
        extractEulerAngleYXZ(glm::mat4x4(*this), euler.Yaw, euler.Pitch, euler.Roll);

        return euler * (isDegrees ? FMath::RAD_TO_DEGREES : 1.f);
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

    void Matrix4x4::SetScale(const Vector3 scale)
    {
        if(const Vector3 prevScale = GetScale(); !Vector3::Equal(prevScale, Vector3::OneVector()))
        {
            if(Vector3::Equal(prevScale, scale)) return;
            _columns[0] /= prevScale.X;
            _columns[1] /= prevScale.Y;
            _columns[2] /= prevScale.Z;
        }

        _columns[0].Normalize();
        _columns[1].Normalize();
        _columns[2].Normalize();

        if(Vector3::Equal(scale, Vector3::OneVector())) return;
        Scale(scale);
    }

    void Matrix4x4::Scale(const Vector3 scale)
    {
        _columns[0] *= FMath::MaxOrMin(scale.X, FMath::EPSILON);
        _columns[1] *= FMath::MaxOrMin(scale.Y, FMath::EPSILON);
        _columns[2] *= FMath::MaxOrMin(scale.Z, FMath::EPSILON);
    }

    Vector3 Matrix4x4::GetAbsoluteScale() const
    {
        const Vector3 scale(Vector3(_columns[0]).Length(), Vector3(_columns[1]).Length(), Vector3(_columns[2]).Length());
        return scale;
    }

    Vector3 Matrix4x4::GetScale() const
    {
        Matrix3x3 copy = this->GetNormalized();
        Vector3 scale = GetAbsoluteScale();

        if(Vector3::Dot(copy[0], Vector3::Cross(copy[1], copy[2])) < 0)
        {
            scale.X *= -1.f;
            scale.Y *= -1.f;
            scale.Z *= -1.f;
        }

        return scale;
    }

    Vector3 Matrix4x4::GetRight() const
    {
        return Vector3(_columns[0]).GetNormalized();
    }

    Vector3 Matrix4x4::GetUp() const
    {
        return Vector3(_columns[1]).GetNormalized();
    }

    Vector3 Matrix4x4::GetForward() const
    {
        return Vector3(_columns[2]).GetNormalized();
    }

    Matrix4x4 Matrix4x4::Perspective(const float fovYRadians, const float aspect, const float zNear, const float zFar)
    {
        return Convert(glm::perspective(fovYRadians, aspect, zNear, zFar));
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

    Vector3 operator*(const Matrix4x4& matrix, const Vector3& vector)
    {
        return {matrix[0][0]*vector.X + matrix[0][1]*vector.Y + matrix[2][0]*vector.Z,
            matrix[0][1]*vector.X + matrix[1][1]*vector.Y + matrix[2][1]*vector.Z,
            matrix[0][2]*vector.X + matrix[1][2]*vector.Y + matrix[2][2]*vector.Z };
    }
}
