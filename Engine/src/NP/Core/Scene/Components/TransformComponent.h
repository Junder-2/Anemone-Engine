#pragma once

namespace Engine
{
    struct TransformComponent
    {
    public:
        glm::mat4 Transform;

        //copy constructor
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        TransformComponent(const glm::mat4& transform) : Transform(transform) {
        }

        // lets you access the Transform, this is a glm::mat4 with extra fuss
         operator const glm::mat4&() const { return Transform; }
         operator glm::mat4&() { return Transform; }
    };
}
