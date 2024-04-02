#pragma once
#include "../../TransformMatrix.h"

namespace Engine
{
    struct TransformComponent
    {
    public:
        TransformMatrix Transform;

        //copy constructor
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        TransformComponent(const TransformMatrix& transform) : Transform(transform) {
        }

        // lets you access the Transform, this is a glm::mat4 with extra fuss
         operator const TransformMatrix&() const { return Transform; }
         operator TransformMatrix&() { return Transform; }
    };
}
