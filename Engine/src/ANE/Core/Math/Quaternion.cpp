#include "anepch.h"
#include "Quaternion.h"

#include <reactphysics3d/mathematics/Quaternion.h>

namespace Engine
{
    Quaternion::operator const reactphysics3d::Quaternion&() const
    {
        return reactphysics3d::Quaternion(X, Y, Z, W);
    }

    Quaternion::operator const glm::quat&() const
    {
        return glm::quat(X, Y, Z, W);
    }

    Quaternion::Quaternion(const Matrix3x3& matrix)
    {
         // Get the trace of the matrix
         const float trace = matrix.GetTrace();

        float r;
        float s;

        if (trace < float(0.0)) {
            if (matrix[1][1] > matrix[0][0]) {
                if(matrix[2][2] > matrix[1][1]) {
                    r = std::sqrt(matrix[2][2] - matrix[0][0] - matrix[1][1] + float(1.0));
                    s = .5f / r;

                    // Compute the quaternion
                    X = (matrix[2][0] + matrix[0][2]) * s;
                    Y = (matrix[1][2] + matrix[2][1]) * s;
                    Z = .5f * r;
                    W = (matrix[1][0] - matrix[0][1]) * s;
                }
                else {
                    r = std::sqrt(matrix[1][1] - matrix[2][2] - matrix[0][0] + float(1.0));
                    s = .5f / r;

                    // Compute the quaternion
                    X = (matrix[0][1] + matrix[1][0]) * s;
                    Y = .5f * r;
                    Z = (matrix[1][2] + matrix[2][1]) * s;
                    W = (matrix[0][2] - matrix[2][0]) * s;
                }
            }
            else if (matrix[2][2] > matrix[0][0]) {
                r = std::sqrt(matrix[2][2] - matrix[0][0] - matrix[1][1] + float(1.0));
                s = .5f / r;

                // Compute the quaternion
                X = (matrix[2][0] + matrix[0][2]) * s;
                Y = (matrix[1][2] + matrix[2][1]) * s;
                Z = .5f * r;
                W = (matrix[1][0] - matrix[0][1]) * s;
            }
            else {
                r = std::sqrt(matrix[0][0] - matrix[1][1] - matrix[2][2] + float(1.0));
                s = .5f / r;

                // Compute the quaternion
                X = .5f * r;
                Y = (matrix[0][1] + matrix[1][0]) * s;
                Z = (matrix[2][0] + matrix[0][2]) * s;
                W = (matrix[2][1] - matrix[1][2]) * s;
            }
        }
        else {
            r = std::sqrt(trace + float(1.0));
            s = .5f / r;

            // Compute the quaternion
            X = (matrix[2][1] - matrix[1][2]) * s;
            Y = (matrix[0][2] - matrix[2][0]) * s;
            Z = (matrix[1][0] - matrix[0][1]) * s;
            W = .5f * r;
        }
    }

    Quaternion Quaternion::FromEulerAngles(float angleX, float angleY, float angleZ)
    {
        Quaternion quaternion;
        quaternion.InitWithEulerAngles(angleX, angleY, angleZ);

        return quaternion;
    }

    Quaternion Quaternion::FromEulerAngles(const Vector3& eulerAngles)
    {
        Quaternion quaternion;
        quaternion.InitWithEulerAngles(eulerAngles.X, eulerAngles.Y, eulerAngles.Z);

        return quaternion;
    }

    void Quaternion::InitWithEulerAngles(const float angleX, const float angleY, const float angleZ)
    {
        float angle = angleX * .5f;
        const float sinX = std::sin(angle);
        const float cosX = std::cos(angle);

        angle = angleY * .5f;
        const float sinY = std::sin(angle);
        const float cosY = std::cos(angle);

        angle = angleZ * .5f;
        const float sinZ = std::sin(angle);
        const float cosZ = std::cos(angle);

        const float cosYcosZ = cosY * cosZ;
        const float sinYcosZ = sinY * cosZ;
        const float cosYsinZ = cosY * sinZ;
        const float sinYsinZ = sinY * sinZ;

        X = sinX * cosYcosZ - cosX * sinYsinZ;
        Y = cosX * sinYcosZ + sinX * cosYsinZ;
        Z = cosX * cosYsinZ - sinX * sinYcosZ;
        W = cosX * cosYcosZ + sinX * sinYsinZ;

        // Normalize the quaternion
        Normalize();
    }
}


