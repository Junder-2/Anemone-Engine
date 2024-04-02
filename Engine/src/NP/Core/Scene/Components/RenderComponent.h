#pragma once

namespace Engine {
    struct RenderComponent {
        // this is a example
        glm::mat3 Renderer;

        //copy constructor
        RenderComponent() = default;
        RenderComponent(const RenderComponent&) = default;

        RenderComponent(const glm::mat3& transform) : Renderer(transform) {
        }

        // lets you access the Transform, this is a glm::mat4 with extra fuss
        operator const glm::mat3&() const { return Renderer; }
        operator glm::mat3&() { return Renderer; }
    };
}
