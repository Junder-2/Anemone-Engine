#pragma once
#include "ANE/Core/Math/Vector4.h"

//Modified copy of reactphysics/include/reactphysics3d/mathematics/Matrix3x3.h


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
            _rows[0] = value;
            _rows[1] = value;
            _rows[2] = value;
            _rows[3] = value;
        }

        Matrix4x4(const float a1, const float a2, const float a3, const float a4,
                    const float b1, const float b2, const float b3, const float b4,
                    const float c1, const float c2, const float c3, const float c4,
                    const float d1, const float d2, const float d3, const float d4)
        {
            _rows[0][0] = a1; _rows[0][1] = a2; _rows[0][2] = a3; _rows[0][3] = a4;
            _rows[1][0] = b1; _rows[1][1] = b2; _rows[1][2] = b3; _rows[1][3] = b4;
            _rows[2][0] = c1; _rows[2][1] = c2; _rows[2][2] = c3; _rows[2][3] = c4;
            _rows[3][0] = d1; _rows[3][1] = d2; _rows[3][2] = d3; _rows[3][3] = d4;
        }

        Matrix4x4(const Vector4 a, const Vector4 b, const Vector4 c, const Vector4 d)
        {
            _rows[0] = a;
            _rows[1] = b;
            _rows[2] = c;
            _rows[3] = d;
        }

        void Clear();

        Vector4 GetColumn(int i) const;
        Vector4 GetRow(int i) const;

        Matrix4x4 GetTranspose() const;
        float GetDeterminant() const;
        Matrix4x4 GetInverse() const;

        void Translate(Vector3 delta);
        void SetPosition(Vector3 newPos);
        Vector3 GetPosition() const;

        void Rotate(float angle, Vector3 axis, bool isDegrees = false);
        void Rotate(Quaternion quat);
        void Rotate(Vector3 euler, bool isDegrees = false);
        void SetRotation(Vector3 euler, bool isDegrees = false);
        void SetRotation(Quaternion quat);
        Quaternion GetQuaternion() const;
        Vector3 GetEulerAngles(bool isDegrees = false) const;

        void Scale(Vector3 scale);
        void SetScale(Vector3 scale);
        Vector3 GetScale() const;

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
            return (_rows[0] == matrix[0] && _rows[1] == matrix[1] && _rows[2] == matrix[2] && _rows[3] == matrix[3]);
        }

        bool operator!= (const Matrix4x4& matrix) const
        {
            return !(*this == matrix);
        }

        Matrix4x4& operator+=(const Matrix4x4& matrix)
        {
            _rows[0] += matrix[0];
            _rows[1] += matrix[1];
            _rows[2] += matrix[2];
            _rows[3] += matrix[3];
            return *this;
        }

        Matrix4x4& operator-=(const Matrix4x4& matrix)
        {
            _rows[0] -= matrix[0];
            _rows[1] -= matrix[1];
            _rows[2] -= matrix[2];
            _rows[3] -= matrix[3];
            return *this;
        }

        Matrix4x4& operator*=(const float scalar)
        {
            _rows[0] *= scalar;
            _rows[1] *= scalar;
            _rows[2] *= scalar;
            _rows[3] *= scalar;
            return *this;
        }

        Matrix4x4& operator*=(const Matrix4x4& matrix);

        const Vector4& operator[](const int row) const
        {
            return _rows[row];
        }

        Vector4& operator[](const int row)
        {
            return _rows[row];
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
            return {matrix[0][0]*vector.X + matrix[0][1]*vector.Y + matrix[0][2]*vector.Z + matrix[0][3]*vector.W,
                    matrix[1][0]*vector.X + matrix[1][1]*vector.Y + matrix[1][2]*vector.Z + matrix[1][3]*vector.W,
                    matrix[2][0]*vector.X + matrix[2][1]*vector.Y + matrix[2][2]*vector.Z + matrix[2][3]*vector.W,
                    matrix[3][0]*vector.X + matrix[3][1]*vector.Y + matrix[3][2]*vector.Z + matrix[3][3]*vector.W};
        }

        std::string ToString() const;

    private:
        Vector4 _rows[4];
    };

    inline void Matrix4x4::Clear()
    {
        _rows[0] = 0;
        _rows[1] = 0;
        _rows[2] = 0;
        _rows[3] = 0;
    }

    inline Vector4 Matrix4x4::GetColumn(int i) const
    {
        assert(i>= 0 && i<3);
        return Vector4(_rows[0][i], _rows[1][i], _rows[2][i]);
    }

    inline Vector4 Matrix4x4::GetRow(int i) const
    {
        assert(i>= 0 && i<3);
        return _rows[i];
    }

    inline std::string Matrix4x4::ToString() const
    {
        return "Matrix4x4(" +
            std::to_string(_rows[0][0]) + "," + std::to_string(_rows[0][1]) + "," + std::to_string(_rows[0][2]) + "," + std::to_string(_rows[0][3]) + "," +
           std::to_string(_rows[1][0]) + "," + std::to_string(_rows[1][1]) + "," + std::to_string(_rows[1][2]) + "," + std::to_string(_rows[1][3]) + "," +
           std::to_string(_rows[2][0]) + "," + std::to_string(_rows[2][1]) + "," + std::to_string(_rows[2][2]) + "," + std::to_string(_rows[2][3]) + "," +
            std::to_string(_rows[3][0]) + "," + std::to_string(_rows[3][1]) + "," + std::to_string(_rows[3][2]) + "," + std::to_string(_rows[3][3]) + "," + ")";
    }
}
