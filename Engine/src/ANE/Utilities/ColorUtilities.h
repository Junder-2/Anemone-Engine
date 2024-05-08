#pragma once
#include "ANE/Math/Types/Vector3.h"

namespace Engine::ColorUtilities
{
    inline Vector3 HexToRGB(const uint32_t value)
    {
        return{
            ((value & 0xff0000) >> 16) / 255.f,
            ((value & 0x00ff00) >> 8) / 255.f,
            ((value & 0x0000ff) >> 0) / 255.f
        };
    }
}
