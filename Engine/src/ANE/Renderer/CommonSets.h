#pragma once
#include "ANE/Core/Math/Vector4.h"
#include "ANE/Core/Math/Matrix/Matrix4x4.h"

namespace Engine
{
    struct ApplicationData
    {
        Vector4 Time;
    };

    struct CameraData
    {
        Vector4 CameraPosition;
        Vector4 ScreenParameters;
    };

    struct SceneData
    {
        Vector4 AmbientColor;

        Vector4 SunlightDirection;
        Vector4 SunlightColor;
    };

    struct TransformationData
    {
        Matrix4x4 LocalToWorld;
        Matrix4x4 LocalToProjection;
    };
}
