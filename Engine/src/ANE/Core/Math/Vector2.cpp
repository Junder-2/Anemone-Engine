#include "anepch.h"
#include "Vector2.h"

#include <reactphysics3d/mathematics/Vector2.h>

namespace Engine
{
    Vector2::operator const reactphysics3d::Vector2&() const
    {
        return reactphysics3d::Vector2(X, Y);
    }

    Vector2::operator const glm::vec2&() const
    {
        return glm::vec2(X, Y);
    }
}
