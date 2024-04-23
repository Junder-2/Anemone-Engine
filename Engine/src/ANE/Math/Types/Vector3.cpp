#include "anepch.h"
#include "Vector3.h"

#include <reactphysics3d/mathematics/Vector3.h>

#include "Vector2.h"
#include "Vector4.h"

namespace Engine
{
    Vector3::Vector3(const Vector2 vector2, const float newZ) : X(vector2.X), Y(vector2.Y), Z(newZ) {}

    Vector3 Vector3::Convert(const reactphysics3d::Vector3& vec)
    {
        return {vec.x, vec.y, vec.z};
    }

    Vector3 Vector3::Convert(const glm::vec3& vec)
    {
        return {vec.x, vec.y, vec.z};
    }

    Vector3::operator const reactphysics3d::Vector3() const
    {
        return reactphysics3d::Vector3(X, Y, Z);
    }

    Vector3::operator const glm::vec3() const
    {
        return glm::vec3(X, Y, Z);
    }

    Vector3::operator const Vector2() const
    {
        return {X, Y};
    }

    Vector3::operator const Vector4() const
    {
        return {X, Y, Z, 0};
    }
}
