#pragma once
#include "Component.h"

namespace Engine
{
    struct UUIDComponent : Component
    {
        std::string UUID{};
        UUIDComponent(std::string uuid) : Component(typeid(*this).name()) , UUID(uuid) {}
    };


}
