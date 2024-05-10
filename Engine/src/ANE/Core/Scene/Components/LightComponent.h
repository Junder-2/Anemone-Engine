#pragma once
#include "Component.h"
#include "ANE/Math/Types/Vector3.h"

namespace Engine
{
    enum LightType : uint8_t
    {
        Directional,
        Point,
        Spotlight
    };

    struct LightComponent
    {
        ANE_COMPONENT_INIT(LightComponent)

    private:
        float _intensity;
        LightType _lightType;
        Vector3 _color;
    };
}
