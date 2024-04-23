#include "anepch.h"
#include "Vector2.h"

#include <reactphysics3d/mathematics/Vector2.h>

#include "imgui.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Engine
{
    Vector2 Vector2::Convert(const reactphysics3d::Vector2& vec)
    {
        return {vec.x, vec.y};
    }

    Vector2 Vector2::Convert(const glm::vec2& vec)
    {
        return {vec.x, vec.y};
    }

    Vector2 Vector2::Convert(const ImVec2& vec)
    {
        return {vec.x, vec.y};
    }

    Vector2::operator const reactphysics3d::Vector2() const
    {
        return reactphysics3d::Vector2(X, Y);
    }

    Vector2::operator const glm::vec2() const
    {
        return glm::vec2(X, Y);
    }

    Vector2::operator const ImVec2() const
    {
        return ImVec2(X, Y);
    }

    Vector2::operator const Vector3() const
    {
        return {X, Y, 0};
    }

    Vector2::operator const Vector4() const
    {
        return {X, Y, 0, 0};
    }
}
