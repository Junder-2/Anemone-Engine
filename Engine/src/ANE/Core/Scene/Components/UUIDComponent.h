#pragma once
#include "Component.h"

namespace Engine
{
    struct UUIDComponent : Component
    {
        ANE_COMPONENT_INIT(UUIDComponent)
        UUIDComponent(const std::string& uuid) : Component(typeid(*this).name()), UUID(uuid) {}
        std::string UUID {};

        static void RegisterComponentMetaData()
        {
            entt::meta<UUIDComponent>()
                .data<&UUIDComponent::UUID>("UUID"_hs).prop("display_name"_hs, "UUID")
                .NOT_EDITABLE;
        }
    };
}
