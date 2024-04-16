#include "anepch.h"
#include "Vector4.h"

#include "imgui.h"
#include "Vector2.h"
#include "Vector3.h"

namespace Engine
{
    Vector4::Vector4(const Vector2 vector2, const float newZ, const float newW) : X(vector2.X), Y(vector2.Y), Z(newZ), W(newW) {}

    Vector4::Vector4(const Vector3 vector3, const float newW) : X(vector3.X), Y(vector3.Y), Z(vector3.Z), W(newW) {}

    Vector4 Vector4::Convert(const glm::vec4& vec)
    {
        return {vec.x, vec.y, vec.z, vec.w};
    }

    Vector4 Vector4::Convert(const ImVec4& vec)
    {
        return {vec.x, vec.y, vec.z, vec.w};
    }

    Vector4::operator const glm::vec4() const
    {
        return glm::vec4(X, Y, Z, W);
    }

    Vector4::operator const ImVec4() const
    {
        return ImVec4(X, Y, Z, W);
    }

    Vector4::operator const Vector2() const
    {
        return {X, Y};
    }

    Vector4::operator const Vector3() const
    {
        return {X, Y, Z};
    }
}
