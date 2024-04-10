#pragma once
#include "ANE/Core/Math/Vector3.h"

namespace Engine
{
    struct Matrix3x3
    {
        /// Constructor
        Matrix3x3();

        /// Constructor
        Matrix3x3(float value)
        {
            Clear();
        }

        /// Constructor
        Matrix3x3(const float a1, const float a2, const float a3, const float b1, const float b2, const float b3, const float c1, const float c2, const float c3)
        {
            _rows[0][0] = a1; _rows[0][1] = a2; _rows[0][2] = a3;
            _rows[1][0] = b1; _rows[1][1] = b2; _rows[1][2] = b3;
            _rows[2][0] = c1; _rows[2][1] = c2; _rows[2][2] = c3;
        }

        Matrix3x3(const Vector3 a, const Vector3 b, const Vector3 c)
        {
            _rows[0][0] = a.X; _rows[0][1] = a.Y; _rows[0][2] = a.Z;
            _rows[1][0] = b.X; _rows[1][1] = b.Y; _rows[1][2] = b.Z;
            _rows[2][0] = c.X; _rows[2][1] = c.Y; _rows[2][2] = c.Z;
        }

        void Clear();

        Vector3 GetColumn(int i) const;

        /// Return a row
        Vector3 GetRow(int i) const;

        /// Return the transpose matrix
        Matrix3x3 GetTranspose() const;

        /// Return the determinant of the matrix
        float GetDeterminant() const;

        /// Return the trace of the matrix
        float GetTrace() const;

        /// Return the inverse matrix
        Matrix3x3 GetInverse() const;

        /// Return the inverse matrix
        Matrix3x3 GetInverse(float determinant) const;

        /// Return the matrix with absolute values
        Matrix3x3 GetAbsoluteMatrix() const;

        /// Return the 3x3 identity matrix
        static Matrix3x3 Identity();

        /// Return the 3x3 zero matrix
        static Matrix3x3 Zero();

        /// Overloaded operator for equality condition
        bool operator==(const Matrix3x3& matrix) const
        {

        }

        /// Overloaded operator for the is different condition
        bool operator!= (const Matrix3x3& matrix) const
        {

        }

        /// Overloaded operator for addition with assignment
        Matrix3x3& operator+=(const Matrix3x3& matrix)
        {

        }

        /// Overloaded operator for substraction with assignment
        Matrix3x3& operator-=(const Matrix3x3& matrix)
        {

        }

        /// Overloaded operator for multiplication with a number with assignment
        Matrix3x3& operator*=(float nb)
        {

        }

        /// Overloaded operator to read element of the matrix.
        const Vector3& operator[](int row) const
        {

        }

        /// Overloaded operator to read/write element of the matrix.
        Vector3& operator[](int row)
        {

        }

        /// Overloaded operator for addition
        Matrix3x3 operator+(const Matrix3x3& matrix1, const Matrix3x3& matrix2)
        {

        }

        /// Overloaded operator for substraction
        Matrix3x3 operator-(const Matrix3x3& matrix1, const Matrix3x3& matrix2)
        {

        }

        /// Overloaded operator for the negative of the matrix
        Matrix3x3 operator-(const Matrix3x3& matrix)
        {

        }

        /// Overloaded operator for multiplication with a number
        Matrix3x3 operator*(float nb, const Matrix3x3& matrix)
        {

        }

        /// Overloaded operator for multiplication with a matrix
        Matrix3x3 operator*(const Matrix3x3& matrix, float nb)
        {

        }

        /// Overloaded operator for matrix multiplication
        Matrix3x3 operator*(const Matrix3x3& matrix1, const Matrix3x3& matrix2)
        {

        }

        /// Overloaded operator for multiplication with a vector
        Vector3 operator*(const Matrix3x3& matrix, const Vector3& vector)
        {

        }

        std::string ToString() const;

    private:
        Vector3 _rows[3];
    };

    inline Matrix3x3::Matrix3x3()
    {
        Clear();
    }

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

    inline Matrix3x3 Matrix3x3::GetTranspose() const
    {
        // Return the transpose matrix
        return {_rows[0][0], _rows[1][0], _rows[2][0],
                    _rows[0][1], _rows[1][1], _rows[2][1],
                    _rows[0][2], _rows[1][2], _rows[2][2]};
    }

    inline float Matrix3x3::GetDeterminant() const
    {
        // Compute and return the determinant of the matrix
        return (_rows[0][0]*(_rows[1][1]*_rows[2][2]-_rows[2][1]*_rows[1][2]) -
                _rows[0][1]*(_rows[1][0]*_rows[2][2]-_rows[2][0]*_rows[1][2]) +
                _rows[0][2]*(_rows[1][0]*_rows[2][1]-_rows[2][0]*_rows[1][1]));
    }

    inline float Matrix3x3::GetTrace() const
    {
        // Compute and return the trace
        return (_rows[0][0] + _rows[1][1] + _rows[2][2]);
    }

    inline Matrix3x3 Matrix3x3::GetInverse() const
    {
        return GetInverse(GetDeterminant());
    }

    inline Matrix3x3 Matrix3x3::GetAbsoluteMatrix() const
    {
        return {std::abs(_rows[0][0]), std::abs(_rows[0][1]), std::abs(_rows[0][2]),
                     std::abs(_rows[1][0]), std::abs(_rows[1][1]), std::abs(_rows[1][2]),
                     std::abs(_rows[2][0]), std::abs(_rows[2][1]), std::abs(_rows[2][2])};
    }

    inline Matrix3x3 Matrix3x3::Identity()
    {
        return {1.0, 0.0, 0.0,
                    0.0, 1.0, 0.0,
                    0.0, 0.0, 1.0};
    }

    inline Matrix3x3 Matrix3x3::Zero()
    {
        return {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    }

    inline std::string Matrix3x3::ToString() const
    {
        return "Matrix3x3(" + std::to_string(_rows[0][0]) + "," + std::to_string(_rows[0][1]) + "," + std::to_string(_rows[0][2]) + "," +
           std::to_string(_rows[1][0]) + "," + std::to_string(_rows[1][1]) + "," + std::to_string(_rows[1][2]) + "," +
           std::to_string(_rows[2][0]) + "," + std::to_string(_rows[2][1]) + "," + std::to_string(_rows[2][2]) + ")";
    }
}
