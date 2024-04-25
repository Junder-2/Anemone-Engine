#pragma once
#include "ANE/Math/Types/Vector4.h"
#include "ANE/Math/Types/Matrix4x4.h"

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
