#pragma once
#include "ANE/Math/Types/Vector3.h"

namespace Engine::ColorUtilities
{
    inline Vector3 HexToRGB(const uint32_t value)
    {
        return{
            ((value & 0xff0000) >> 16) / 256.f,
            ((value & 0x00ff00) >> 8) / 256.f,
            ((value & 0x0000ff) >> 0) / 256.f
        };
    }
}
