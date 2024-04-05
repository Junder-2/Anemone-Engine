#pragma once
#include "Component.h"

namespace Engine
{
    struct TagComponent : Component
    {
    public:
        const char* Tag;

        ANE_COMPONENT_INIT(TagComponent)

        TagComponent(const char* tag) : Component(typeid(*this).name()) , Tag(tag) {}

        operator const char*&() { return Tag; }
    };
}
