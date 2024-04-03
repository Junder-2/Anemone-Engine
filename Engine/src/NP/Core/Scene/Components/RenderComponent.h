﻿#pragma once
#include "Component.h"

namespace Engine
{
    struct RenderComponent : Component
    {
        // this is a example
        glm::mat3 Renderer;

        //copy constructor
        RenderComponent() : Component(typeid(*this).name()) {}
        RenderComponent(const RenderComponent&) = default;

        RenderComponent(const glm::mat3& transform) : Component(typeid(*this).name()) , Renderer(transform) {

        }

        // lets you access the Transform, this is a glm::mat4 with extra fuss
        operator const glm::mat3&() const { return Renderer; }
        operator glm::mat3&() { return Renderer; }
    };
}
