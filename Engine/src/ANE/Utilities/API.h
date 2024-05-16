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

        /** Returns the global time scale */
        inline static float TIME_SCALE = 1.f;
        /** Returns the scaled application time in seconds */
        inline static float TIME = 0.f;
        /** Returns the unscaled application time in seconds */
        inline static float TIME_UNSCALED = 0.f;
        /** Returns the global scaled deltaTime this frame */
        inline static float DELTA_TIME = 0.f;
        /** Returns the global unscaled deltaTime this frame */
        inline static float DELTA_TIME_UNSCALED = 0.f;

        /** Returns the global timestep used for physics */
        inline static float PHYSICS_TIMESTEP = 1/50.f;
    };
};
