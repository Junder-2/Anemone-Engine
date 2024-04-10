#include "anepch.h"
#include "Vector3.h"

#include <reactphysics3d/mathematics/Vector3.h>

namespace Engine
{
    Vector3::operator const reactphysics3d::Vector3&() const
    {
        return reactphysics3d::Vector3(X, Y, Z);
    }

    Vector3::operator const glm::vec3&() const
    {
        return glm::vec3(X, Y, Z);
    }
}
