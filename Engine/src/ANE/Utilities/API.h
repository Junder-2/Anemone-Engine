#pragma once
#include "ANE/Math/VectorTypes.h"

namespace Engine
{
    enum Platform
    {
        Windows,
        Linux,
        MacOS
    };

    class API
    {
    public:
        /** Returns window size in pixels */
        inline static Vector2 WINDOW_SIZE = Vector2::ZeroVector();
        /** Returns window position in pixels */
        inline static Vector2 WINDOW_POS = Vector2::ZeroVector();

        /** Returns viewport size in pixels */
        inline static Vector2 VIEWPORT_SIZE = Vector2::ZeroVector();
        /** Returns viewport position Relative to window in pixels */
        inline static Vector2 VIEWPORT_POS = Vector2::ZeroVector();

        inline static Platform PLATFORM = Platform::Windows;

        inline static float PHYSICS_TIMESTEP = 1/50.f;
    };
};
