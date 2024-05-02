#pragma once
#include "Component.h"
namespace Engine
{
    using namespace entt::literals;
    struct TagComponent : Component
    {
    public:
        //const char* Tag;
        std::string Value;

        ANE_COMPONENT_INIT(TagComponent)

        TagComponent(const char* tag) : Component(typeid(*this).name()) , Value(tag) {}

        static void RegisterComponentMetaData()
        {
            entt::meta<TagComponent>()
                   .data<&TagComponent::Value>("Tag"_hs).prop("display_name"_hs, "Tag")
                   .EDITABLE;

        }

        //operator const char*&() { return Tag; }
    };


}
