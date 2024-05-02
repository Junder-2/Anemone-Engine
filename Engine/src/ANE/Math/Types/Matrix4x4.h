//Modified copy of reactphysics/include/reactphysics3d/mathematics/Matrix3x3.h

#pragma once
#include "ANE/Math/Types/Vector4.h"

namespace reactphysics3d
{
    class Matrix4x4;
}

namespace Engine
{
    struct Matrix3x3;
    struct Quaternion;

    struct Matrix4x4
    {
        Matrix4x4()
        {
            Clear();
        }

        Matrix4x4(const float value)
        {
            _columns[0] = value;
            _columns[1] = value;
            _columns[2] = value;
            _columns[3] = value;
        }

        Matrix4x4(const float a1, const float a2, const float a3, const float a4,
                    const float b1, const float b2, const float b3, const float b4,
                    const float c1, const float c2, const float c3, const float c4,
                    const float d1, const float d2, const float d3, const float d4)
        {
            _columns[0][0] = a1; _columns[0][1] = a2; _columns[0][2] = a3; _columns[0][3] = a4;
            _columns[1][0] = b1; _columns[1][1] = b2; _columns[1][2] = b3; _columns[1][3] = b4;
            _columns[2][0] = c1; _columns[2][1] = c2; _columns[2][2] = c3; _columns[2][3] = c4;
            _columns[3][0] = d1; _columns[3][1] = d2; _columns[3][2] = d3; _columns[3][3] = d4;
        }

        Matrix4x4(const Vector4 a, const Vector4 b, const Vector4 c, const Vector4 d)
        {
            _columns[0] = a;
            _columns[1] = b;
            _columns[2] = c;
            _columns[3] = d;
        }

        void Clear();

        Vector4 GetColumn(int i) const;
        Vector4 GetRow(int i) const;

        Matrix4x4 GetTranspose() const;
        float GetDeterminant() const;
        Matrix4x4 GetInverse() const;
        Matrix4x4 GetNormalized() const;

        void SetPosition(Vector3 newPos);
        void Translate(Vector3 delta);
        void AddPosition(Vector3 delta);
        Vector3 GetPosition() const;

        void SetRotation(const Quaternion& quat);
        void Rotate(const Quaternion& quat);
        Quaternion GetQuaternion() const;

        void SetRotation(Vector3 euler, bool isDegrees = false);
        void Rotate(float angle, Vector3 axis, bool isDegrees = false);
        void Rotate(Vector3 euler, bool isDegrees = false);
        Vector3 GetEulerAngles(bool isDegrees = false) const;

        void SetScale(Vector3 scale);
        void Scale(Vector3 scale);
        Vector3 GetScale() const;

        Vector3 GetRight() const;
        Vector3 GetUp() const;
        Vector3 GetForward() const;

        static Matrix4x4 Identity()
        {
            return {1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1};
        }

        static Matrix4x4 Zero() { return {0}; }

        // Conversion to other matrix types
        static Matrix4x4 Convert(const glm::mat4& mat4);
        operator const glm::mat4() const;

        operator const Matrix3x3() const;

        bool operator==(const Matrix4x4& matrix) const
        {
            return (_columns[0] == matrix[0] && _columns[1] == matrix[1] && _columns[2] == matrix[2] && _columns[3] == matrix[3]);
        }

        bool operator!= (const Matrix4x4& matrix) const
        {
            return !(*this == matrix);
        }

        Matrix4x4& operator+=(const Matrix4x4& matrix)
        {
            _columns[0] += matrix[0];
            _columns[1] += matrix[1];
            _columns[2] += matrix[2];
            _columns[3] += matrix[3];
            return *this;
        }

        Matrix4x4& operator-=(const Matrix4x4& matrix)
        {
            _columns[0] -= matrix[0];
            _columns[1] -= matrix[1];
            _columns[2] -= matrix[2];
            _columns[3] -= matrix[3];
            return *this;
        }

        Matrix4x4& operator*=(const float scalar)
        {
            _columns[0] *= scalar;
            _columns[1] *= scalar;
            _columns[2] *= scalar;
            _columns[3] *= scalar;
            return *this;
        }

        Matrix4x4& operator*=(const Matrix4x4& matrix);

        const Vector4& operator[](const int row) const
        {
            return _columns[row];
        }

        Vector4& operator[](const int row)
        {
            return _columns[row];
        }

        friend Matrix4x4 operator+(const Matrix4x4& matrix1, const Matrix4x4& matrix2)
        {
            return {matrix1[0] + matrix2[0],
                        matrix1[1] + matrix2[1],
                        matrix1[2] + matrix2[2],
                        matrix1[3] + matrix2[3]};
        }

        friend Matrix4x4 operator-(const Matrix4x4& matrix1, const Matrix4x4& matrix2)
        {
            return {matrix1[0] - matrix2[0],
                    matrix1[1] - matrix2[1],
                    matrix1[2] - matrix2[2],
                    matrix1[3] - matrix2[3]};
        }

        friend Matrix4x4 operator-(const Matrix4x4& matrix)
        {
            return {-matrix[0], -matrix[1], -matrix[2], -matrix[3]};
        }

        friend Matrix4x4 operator*(const float scalar, const Matrix4x4& matrix)
        {
            return {matrix[0] * scalar,
                    matrix[1] * scalar,
                    matrix[2] * scalar,
                    matrix[3] * scalar};
        }

        friend Matrix4x4 operator*(const Matrix4x4& matrix, const float scalar)
        {
            return scalar * matrix;
        }

        friend Matrix4x4 operator*(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

        friend Vector4 operator*(const Matrix4x4& matrix, const Vector4& vector)
        {
            return {matrix[0][0]*vector.X + matrix[0][1]*vector.Y + matrix[2][0]*vector.Z + matrix[3][0]*vector.W,
                    matrix[0][1]*vector.X + matrix[1][1]*vector.Y + matrix[2][1]*vector.Z + matrix[3][1]*vector.W,
                    matrix[0][2]*vector.X + matrix[1][2]*vector.Y + matrix[2][2]*vector.Z + matrix[3][2]*vector.W,
                    matrix[0][3]*vector.X + matrix[1][3]*vector.Y + matrix[2][3]*vector.Z + matrix[3][3]*vector.W};
        }

        std::string ToString() const;

    private:
        Vector4 _columns[4];
    };

    inline void Matrix4x4::Clear()
    {
        _columns[0] = 0;
        _columns[1] = 0;
        _columns[2] = 0;
        _columns[3] = 0;
    }

    inline Vector4 Matrix4x4::GetColumn(const int i) const
    {
        ANE_EASSERT(i>=0 && i<4, "Trying to return out of bounds column: {}", i);
        return _columns[i];
    }

    inline Vector4 Matrix4x4::GetRow(const int i) const
    {
        ANE_EASSERT(i>=0 && i<4, "Trying to return out of bounds row: {}", i);
        return { _columns[0][i], _columns[1][i], _columns[2][i], _columns[3][i] };
    }

    inline std::string Matrix4x4::ToString() const
    {
        return "Matrix4x4(" +
            std::to_string(_columns[0][0]) + "," + std::to_string(_columns[0][1]) + "," + std::to_string(_columns[0][2]) + "," + std::to_string(_columns[0][3]) + "," +
           std::to_string(_columns[1][0]) + "," + std::to_string(_columns[1][1]) + "," + std::to_string(_columns[1][2]) + "," + std::to_string(_columns[1][3]) + "," +
           std::to_string(_columns[2][0]) + "," + std::to_string(_columns[2][1]) + "," + std::to_string(_columns[2][2]) + "," + std::to_string(_columns[2][3]) + "," +
            std::to_string(_columns[3][0]) + "," + std::to_string(_columns[3][1]) + "," + std::to_string(_columns[3][2]) + "," + std::to_string(_columns[3][3]) + "," + ")";
    }
}
