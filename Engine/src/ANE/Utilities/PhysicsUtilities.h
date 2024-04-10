#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "ANE/Core/Math/TransformMatrix.h"

namespace Engine
{
    using namespace reactphysics3d;

    namespace PhysicsUtilities
    {
        inline Vector3 GLMToReact(const glm::vec3& vec3)
        {
            return {vec3.x, vec3.y, vec3.z};
        }

        inline Vector3 operator Vector3(glm::vec3& vec3)
        {
            return {vec3.x, vec3.y, vec3.z};
        }

        inline Vector3& operator Vector3(const glm::vec3& vec3)
        {
            return {vec3.x, vec3.y, vec3.z};
        }
        //
        // inline glm::vec3 operator glm::vec3&(const Vector3& vec3)
        // {
        //     return {vec3.x, vec3.y, vec3.z};
        // }
        //
        // inline Quaternion operator() (const glm::quat& quat)
        // {
        //     return {quat.x, quat.y, quat.z, quat.w};
        // }
        //
        // inline Transform operator() (const TransformMatrix& matrix)
        // {
        //     Vector3 vec3 = glm::vec3(0, 0, 0);
        //     return {matrix.GetPosition()()};
        // }
        //
        // inline Transform operator ()(const glm::mat4& mat4)
        // {
        //     Vector3 vec3 = mat4[3];
        //     return {vec3, };
        // }

    };
}
