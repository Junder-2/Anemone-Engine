#pragma once
#include <entt.hpp>

using namespace entt::literals;

#define EDITABLEHASH "is_editable"_hs
#define EDITABLE prop(EDITABLEHASH, true)
#define NOT_EDITABLE prop(EDITABLEHASH, false)
