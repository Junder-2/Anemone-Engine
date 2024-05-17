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

    struct LightComponent : Component
    {
        ANE_COMPONENT_INIT(LightComponent)

        static void RegisterComponentMetaData()
        {
            entt::meta<LightComponent>()
                .EDITABLE;
        }

    public:
        float GetIntensity() const { return _intensity; }
        Vector3 GetColor() const { return _color; }

    private:
        float _intensity = 1.f;
        LightType _lightType = Directional;
        Vector3 _color = 1.f;
    };
}
