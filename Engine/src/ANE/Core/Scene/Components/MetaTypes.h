#pragma once
#include <entt.hpp>

using namespace entt::literals;

#define EDITABLE_HASH "is_editable"_hs
#define EDITABLE prop(EDITABLE_HASH, true)
#define NOT_EDITABLE prop(EDITABLE_HASH, false)
