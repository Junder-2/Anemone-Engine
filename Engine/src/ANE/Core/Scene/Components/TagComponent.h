#pragma once
#include "Component.h"

namespace Engine
{
    struct TagComponent : Component
    {
        std::string Value;

        ANE_COMPONENT_INIT(TagComponent)

        TagComponent(const char* tag) : Component(typeid(*this).name()) , Value(tag) {}

        static void RegisterComponentMetaData()
        {
            entt::meta<TagComponent>()
                .data<&TagComponent::Value>("Tag"_hs).prop("display_name"_hs, "Tag")
                .EDITABLE;
        }
    };
}
