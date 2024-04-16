#pragma once
#include "ANE/Core/Math/Vector3.h"

//Modified copy of reactphysics/include/reactphysics3d/mathematics/Matrix3x3.h

namespace reactphysics3d
{
    class Matrix3x3;
}

namespace Engine
{
    struct Matrix4x4;
    struct Quaternion;

    struct Matrix3x3
    {
        Matrix3x3()
        {
            Clear();
        }

        Matrix3x3(const float value)
        {
            _rows[0] = value;
            _rows[1] = value;
            _rows[2] = value;
        }

        Matrix3x3(const float a1, const float a2, const float a3, const float b1, const float b2, const float b3, const float c1, const float c2, const float c3)
        {
            _rows[0][0] = a1; _rows[0][1] = a2; _rows[0][2] = a3;
            _rows[1][0] = b1; _rows[1][1] = b2; _rows[1][2] = b3;
            _rows[2][0] = c1; _rows[2][1] = c2; _rows[2][2] = c3;
        }

        Matrix3x3(const Vector3 a, const Vector3 b, const Vector3 c)
        {
            _rows[0] = a;
            _rows[1] = b;
            _rows[2] = c;
        }

        void Clear();

        Vector3 GetColumn(int i) const;
        Vector3 GetRow(int i) const;

        Matrix3x3 GetTranspose() const;
        float GetDeterminant() const;
        Matrix3x3 GetInverse() const;

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

        Vector3 GetRight() const;
        Vector3 GetUp() const;
        Vector3 GetForward() const;

        static Matrix3x3 Identity()
        {
            return {1.0, 0.0, 0.0,
                    0.0, 1.0, 0.0,
                    0.0, 0.0, 1.0};
        }

        static Matrix3x3 Zero() { return {0}; }

        static Matrix3x3 Convert(const glm::mat3& mat3);

        // Conversion to other vector2 types
        operator const reactphysics3d::Matrix3x3() const;
        operator const glm::mat3() const;

        operator const Matrix4x4() const;

        bool operator==(const Matrix3x3& matrix) const
        {
            return (_rows[0] == matrix[0] && _rows[1] == matrix[1] && _rows[2] == matrix[2]);
        }

        bool operator!= (const Matrix3x3& matrix) const
        {
            return !(*this == matrix);
        }

        Matrix3x3& operator+=(const Matrix3x3& matrix)
        {
            _rows[0] += matrix[0];
            _rows[1] += matrix[1];
            _rows[2] += matrix[2];
            return *this;
        }

        Matrix3x3& operator-=(const Matrix3x3& matrix)
        {
            _rows[0] -= matrix[0];
            _rows[1] -= matrix[1];
            _rows[2] -= matrix[2];
            return *this;
        }

        Matrix3x3& operator*=(const float scalar)
        {
            _rows[0] *= scalar;
            _rows[1] *= scalar;
            _rows[2] *= scalar;
            return *this;
        }

        Matrix3x3& operator*=(const Matrix3x3& matrix);

        const Vector3& operator[](const int row) const
        {
            return _rows[row];
        }

        Vector3& operator[](const int row)
        {
            return _rows[row];
        }

        friend Matrix3x3 operator+(const Matrix3x3& matrix1, const Matrix3x3& matrix2)
        {
            return {matrix1[0] + matrix2[0],
                        matrix1[1] + matrix2[1],
                        matrix1[2] + matrix2[2]};
        }

        friend Matrix3x3 operator-(const Matrix3x3& matrix1, const Matrix3x3& matrix2)
        {
            return {matrix1[0] - matrix2[0],
                    matrix1[1] - matrix2[1],
                    matrix1[2] - matrix2[2]};
        }

        friend Matrix3x3 operator-(const Matrix3x3& matrix)
        {
            return {-matrix[0], -matrix[1], -matrix[2] };
        }

        friend Matrix3x3 operator*(float scalar, const Matrix3x3& matrix)
        {
            return {matrix[0] * scalar,
                    matrix[1] * scalar,
                    matrix[2] * scalar};
        }

        friend Matrix3x3 operator*(const Matrix3x3& matrix, float scalar)
        {
            return scalar * matrix;
        }

        friend Matrix3x3 operator*(const Matrix3x3& matrix1, const Matrix3x3& matrix2);

        friend Vector3 operator*(const Matrix3x3& matrix, const Vector3& vector)
        {
            return {matrix[0][0]*vector.X + matrix[0][1]*vector.Y + matrix[0][2]*vector.Z,
                    matrix[1][0]*vector.X + matrix[1][1]*vector.Y + matrix[1][2]*vector.Z,
                   matrix[2][0]*vector.X + matrix[2][1]*vector.Y + matrix[2][2]*vector.Z};
        }

        std::string ToString() const;

    private:
        Vector3 _rows[3];
    };

    inline void Matrix3x3::Clear()
    {
        _rows[0] = 0;
        _rows[1] = 0;
        _rows[2] = 0;
    }

    inline Vector3 Matrix3x3::GetColumn(int i) const
    {
        assert(i>= 0 && i<3);
        return Vector3(_rows[0][i], _rows[1][i], _rows[2][i]);
    }

    inline Vector3 Matrix3x3::GetRow(int i) const
    {
        assert(i>= 0 && i<3);
        return _rows[i];
    }

    inline std::string Matrix3x3::ToString() const
    {
        return "Matrix3x3(" + std::to_string(_rows[0][0]) + "," + std::to_string(_rows[0][1]) + "," + std::to_string(_rows[0][2]) + "," +
           std::to_string(_rows[1][0]) + "," + std::to_string(_rows[1][1]) + "," + std::to_string(_rows[1][2]) + "," +
           std::to_string(_rows[2][0]) + "," + std::to_string(_rows[2][1]) + "," + std::to_string(_rows[2][2]) + ")";
    }
}
