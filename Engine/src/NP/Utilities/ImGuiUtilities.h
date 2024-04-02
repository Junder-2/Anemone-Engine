#pragma once

struct ImGuiStyle;

namespace Engine
{
    class ImGuiUtilities
    {
    public:
        static void StyleNPDark(ImGuiStyle* dst = nullptr);
    };
}
