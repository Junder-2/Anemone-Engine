#pragma once
#include "Component.h"
#include "ANE/Math/Types/Vector3.h"

namespace Engine
{
    enum class LightType
    {
        Directional,
        Point,
        Spotlight
    };

    struct LightData //Todo add Range etc.
    {
        LightType LightType = LightType::Directional;
        float Intensity = 1.f;
        Vector3 Color = 1.f;
    };

    struct LightComponent : Component
    {
        ANE_COMPONENT_INIT(LightComponent)

        static void RegisterComponentMetaData()
        {
            entt::meta<LightComponent>()
                .data<&LightComponent::_lightData>("LightData"_hs).prop("display_name"_hs, "Light Data")
                .EDITABLE;
        }

        void SetLightType(const LightType type) { _lightData.LightType = type; }
        LightType GetLightType() const { return _lightData.LightType; }

        void SetIntensity(const float intensity) { _lightData.Intensity = intensity; }
        float GetIntensity() const { return _lightData.Intensity; }

        void SetColor(const Vector3 color) { _lightData.Color = color; }
        Vector3 GetColor() const { return _lightData.Color; }

    private:
        LightData _lightData;
    };
}
