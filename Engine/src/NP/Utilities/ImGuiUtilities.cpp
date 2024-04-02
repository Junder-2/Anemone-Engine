#include "nppch.h"
#include "ImGuiUtilities.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace Engine
{
    void ImGuiUtilities::StyleNPDark(ImGuiStyle* dst)
    {
        // ImGuiIO io = ImGui::GetIO();
        // io.Fonts->AddFontFromFileTTF("../Engine/vendor/imgui/misc/fonts/Cousine-Regular.ttf", 14);

        ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        style->WindowRounding = 6.f;
        style->ChildRounding = 6.f;
        style->FrameRounding = 6.f;
        style->PopupRounding = 6.f;
        style->GrabRounding = 10.f;
        style->ScrollbarRounding = 10.f;

        style->WindowPadding = ImVec2(10,8);
        style->FramePadding = ImVec2(5,3);
        style->WindowBorderSize = 1.f;

        constexpr auto imBlack = ImVec4(0, 0, 0, 1);
        constexpr auto imWhite = ImVec4(1, 1, 1, 1);
        constexpr auto imClear = ImVec4(0, 0, 0, 0);

        constexpr ImVec4 npWhite = ImVec4(0.86f, 0.87f, 0.98f, 1.00f);
        constexpr ImVec4 npBlack = ImVec4(0.10f, 0.11f, 0.18f, 1.00f);
        constexpr ImVec4 npBlackBg = ImVec4(0.10f, 0.11f, 0.18f, .98f);
        constexpr ImVec4 npDark = ImVec4(0.20f, 0.22f, 0.36f, 1.00f);
        const ImVec4 npDarkHover = ImLerp(npDark, imBlack, .1f);
        const ImVec4 npDarkActive = ImLerp(npDark, imBlack, .2f);
        constexpr ImVec4 npColor = ImVec4(0.25f, 0.30f, 0.55f, 1.00f);
        const ImVec4 npColorHover = ImLerp(npColor, imBlack, .1f);
        const ImVec4 npColorActive = ImLerp(npColor, imBlack, .2f);
        constexpr ImVec4 npHint = ImVec4(0.57f, 0.62f, 0.87f, 1.00f);
        const ImVec4 npHintHover = ImLerp(npHint, imBlack, .1f);
        const ImVec4 npHintActive = ImLerp(npHint, imBlack, .2f);
        const ImVec4 npBorder = ImLerp(imClear, npBlack, .5f);
        const ImVec4 npBorderHover = ImLerp(npBorder, imBlack, .1f);
        const ImVec4 npBorderActive = ImLerp(npBorder, imBlack, .2f);

        colors[ImGuiCol_Text]                   = npWhite;
        colors[ImGuiCol_TextDisabled]           = ImLerp(imBlack, npWhite, .95f);

        colors[ImGuiCol_WindowBg]               = npBlackBg;
        colors[ImGuiCol_ChildBg]                = npBlackBg;
        colors[ImGuiCol_PopupBg]                = npBlackBg;

        colors[ImGuiCol_Border]                 = ImLerp(npColor, imClear, .5f);
        colors[ImGuiCol_BorderShadow]           = imClear;
        colors[ImGuiCol_FrameBg]                = npBorder;
        colors[ImGuiCol_FrameBgHovered]         = npBorderHover;
        colors[ImGuiCol_FrameBgActive]          = npBorderActive;
        colors[ImGuiCol_TitleBg]                = npDark;
        colors[ImGuiCol_TitleBgActive]          = npDarkHover;
        colors[ImGuiCol_TitleBgCollapsed]       = npDarkActive;
        colors[ImGuiCol_MenuBarBg]              = npDark;

        colors[ImGuiCol_ScrollbarBg]            = npBorder;
        colors[ImGuiCol_ScrollbarGrab]          = npColor;
        colors[ImGuiCol_ScrollbarGrabHovered]   = npColorHover;
        colors[ImGuiCol_ScrollbarGrabActive]    = npColorActive;

        colors[ImGuiCol_CheckMark]              = npHint;
        colors[ImGuiCol_SliderGrab]             = npHint;
        colors[ImGuiCol_SliderGrabActive]       = npHintActive;

        colors[ImGuiCol_Button]                 = npColor;
        colors[ImGuiCol_ButtonHovered]          = npColorHover;
        colors[ImGuiCol_ButtonActive]           = npColorActive;

        colors[ImGuiCol_Header]                 = npDark;
        colors[ImGuiCol_HeaderHovered]          = npDarkHover;
        colors[ImGuiCol_HeaderActive]           = npDarkActive;

        colors[ImGuiCol_Separator]              = npBorder;
        colors[ImGuiCol_SeparatorHovered]       = npBorderHover;
        colors[ImGuiCol_SeparatorActive]        = npBorderActive;

        colors[ImGuiCol_ResizeGrip]             = npColor;
        colors[ImGuiCol_ResizeGripHovered]      = npColorHover;
        colors[ImGuiCol_ResizeGripActive]       = npColorActive;
        colors[ImGuiCol_Tab]                    = npDark;
        colors[ImGuiCol_TabHovered]             = npDarkHover;
        colors[ImGuiCol_TabActive]              = npDarkActive;
        colors[ImGuiCol_TabUnfocused]           = ImLerp(imBlack, npDarkHover, 0.9f);
        colors[ImGuiCol_TabUnfocusedActive]     = ImLerp(imBlack, npDarkHover, 0.8f);

        colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


        // colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        // colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        // colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        // colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        // colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        // colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        // colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        // colors[ImGuiCol_FrameBg]                = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
        // colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        // colors[ImGuiCol_FrameBgActive]          = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        // colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        // colors[ImGuiCol_TitleBgActive]          = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
        // colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        // colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        // colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        // colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        // colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        // colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        // colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        // colors[ImGuiCol_SliderGrab]             = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
        // colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        // colors[ImGuiCol_Button]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        // colors[ImGuiCol_ButtonHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        // colors[ImGuiCol_ButtonActive]           = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
        // colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        // colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        // colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        // colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
        // colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        // colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        // colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
        // colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        // colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        // colors[ImGuiCol_Tab]                    = ImLerp(colors[ImGuiCol_Header],       colors[ImGuiCol_TitleBgActive], 0.80f);
        // colors[ImGuiCol_TabHovered]             = colors[ImGuiCol_HeaderHovered];
        // colors[ImGuiCol_TabActive]              = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
        // colors[ImGuiCol_TabUnfocused]           = ImLerp(colors[ImGuiCol_Tab],          colors[ImGuiCol_TitleBg], 0.80f);
        // colors[ImGuiCol_TabUnfocusedActive]     = ImLerp(colors[ImGuiCol_TabActive],    colors[ImGuiCol_TitleBg], 0.40f);
        // colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        // colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        // colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        // colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        // colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        // colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
        // colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
        // colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        // colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        // colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        // colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        // colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        // colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        // colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        // colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }
}
