#pragma once
#include "ANE/Math/VectorTypes.h"

namespace Engine
{
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

    };


}
