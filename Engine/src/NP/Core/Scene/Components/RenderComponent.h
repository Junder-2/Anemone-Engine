#pragma once

namespace Engine
{
    struct RenderComponent
    {
        // this is a example
        glm::mat4 Renderer;

        //copy constructor
        RenderComponent() = default;
        RenderComponent(const RenderComponent&) = default;

        RenderComponent(const glm::mat4& transform) : Renderer(transform) {
        }

        // lets you access the Transform, this is a glm::mat4 with extra fuss
        operator const glm::mat4&() const { return Renderer; }
        operator glm::mat4&() { return Renderer; }
    };
}
