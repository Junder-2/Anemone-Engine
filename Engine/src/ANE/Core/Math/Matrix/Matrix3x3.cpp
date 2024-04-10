#include "anepch.h"
#include "Matrix3x3.h"

namespace Engine
{
    Matrix3x3 Matrix3x3::GetInverse(const float determinant) const
    {
        // Check if the determinant is equal to zero
        assert(determinant != 0);

        const float invDeterminant = 1.f / determinant;

        Matrix3x3 tempMatrix((_rows[1][1]*_rows[2][2]-_rows[2][1]*_rows[1][2]),
                             -(_rows[0][1]*_rows[2][2]-_rows[2][1]*_rows[0][2]),
                             (_rows[0][1]*_rows[1][2]-_rows[0][2]*_rows[1][1]),
                                -(_rows[1][0]*_rows[2][2]-_rows[2][0]*_rows[1][2]),
                             (_rows[0][0]*_rows[2][2]-_rows[2][0]*_rows[0][2]),
                             -(_rows[0][0]*_rows[1][2]-_rows[1][0]*_rows[0][2]),
                                (_rows[1][0]*_rows[2][1]-_rows[2][0]*_rows[1][1]),
                             -(_rows[0][0]*_rows[2][1]-_rows[2][0]*_rows[0][1]),
                             (_rows[0][0]*_rows[1][1]-_rows[0][1]*_rows[1][0]));

        // Return the inverse matrix
        return (invDeterminant * tempMatrix);
    }
}


