#include "anepch.h"
#include "ImGuiUtilities.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace Engine
{
    Vector4 ImGuiUtilities::_aneStyleColor[AneColorCount];

    void ImGuiUtilities::StyleAnemoneDark(ImGuiStyle* dst)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto fontConfig = ImFontConfig();
        io.Fonts->AddFontFromFileTTF("../Assets/Fonts/Office Code Pro/OfficeCodePro-Regular.otf", 15, &fontConfig);
        io.Fonts->AddFontFromFileTTF("../Assets/Fonts/Office Code Pro/OfficeCodePro-Medium.otf", 15, &fontConfig);

        ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        style->WindowRounding = 4.f;
        style->ChildRounding = 4.f;
        style->FrameRounding = 2.f;
        style->PopupRounding = 2.f;
        style->ScrollbarRounding = 10.f;
        style->GrabRounding = 1.f;
        style->TabRounding = 4.f;

        style->WindowPadding = ImVec2(10,8);
        style->FramePadding = ImVec2(5,3);
        style->WindowMenuButtonPosition = ImGuiDir_None;

        style->WindowBorderSize = 0;
        style->ChildBorderSize = 1;
        style->PopupBorderSize = 1;
        style->FrameBorderSize = 0;
        style->TabBorderSize = 0;
        style-> TabBarBorderSize = 0;

        constexpr auto imBlack = ImVec4(0, 0, 0, 1);
        constexpr auto imWhite = ImVec4(1, 1, 1, 1);
        constexpr auto imClear = ImVec4(0, 0, 0, 0);

        constexpr ImVec4 aneWhite = ImVec4(0.86f, 0.87f, 0.98f, 1.00f);
        constexpr ImVec4 aneBlack = ImVec4(0.10f, 0.11f, 0.18f, 1.00f);
        constexpr ImVec4 aneBlackBg = ImVec4(0.10f, 0.11f, 0.18f, .98f);
        constexpr ImVec4 aneDark = ImVec4(0.20f, 0.22f, 0.36f, 1.00f);
        const ImVec4 aneDarkHover = ImLerp(aneDark, imBlack, .1f);
        const ImVec4 aneDarkActive = ImLerp(aneDark, imBlack, .2f);
        const ImVec4 aneColorHover = ImLerp(aneColor, imBlack, .1f);
        const ImVec4 aneColorActive = ImLerp(aneColor, imBlack, .2f);
        constexpr ImVec4 aneHint = ImVec4(0.57f, 0.62f, 0.87f, 1.00f);
        const ImVec4 aneHintHover = ImLerp(aneHint, imBlack, .1f);
        const ImVec4 aneHintActive = ImLerp(aneHint, imBlack, .2f);
        const ImVec4 aneFrame = ImLerp(imClear, aneBlack, .5f);
        const ImVec4 aneFrameHover = ImLerp(aneFrame, imBlack, .1f);
        const ImVec4 aneFrameActive = ImLerp(aneFrame, imBlack, .2f);

        colors[ImGuiCol_Text]                   = aneWhite;
        colors[ImGuiCol_TextDisabled]           = ImLerp(imBlack, aneWhite, .95f);

        colors[ImGuiCol_WindowBg]               = aneBlackBg;
        colors[ImGuiCol_ChildBg]                = aneBlackBg;
        colors[ImGuiCol_PopupBg]                = aneBlackBg;

        colors[ImGuiCol_Border]                 = aneFrameActive;
        colors[ImGuiCol_BorderShadow]           = imClear;
        colors[ImGuiCol_FrameBg]                = aneFrame;
        colors[ImGuiCol_FrameBgHovered]         = aneFrameHover;
        colors[ImGuiCol_FrameBgActive]          = aneFrameActive;
        colors[ImGuiCol_TitleBg]                = aneDark;
        colors[ImGuiCol_TitleBgActive]          = aneDarkHover;
        colors[ImGuiCol_TitleBgCollapsed]       = aneDarkActive;
        colors[ImGuiCol_MenuBarBg]              = aneDark;

        colors[ImGuiCol_ScrollbarBg]            = aneFrame;
        colors[ImGuiCol_ScrollbarGrab]          = aneColor;
        colors[ImGuiCol_ScrollbarGrabHovered]   = aneColorHover;
        colors[ImGuiCol_ScrollbarGrabActive]    = aneColorActive;

        colors[ImGuiCol_CheckMark]              = aneHint;
        colors[ImGuiCol_SliderGrab]             = aneHint;
        colors[ImGuiCol_SliderGrabActive]       = aneHintActive;

        colors[ImGuiCol_Button]                 = aneColor;
        colors[ImGuiCol_ButtonHovered]          = aneColorHover;
        colors[ImGuiCol_ButtonActive]           = aneColorActive;

        colors[ImGuiCol_Header]                 = aneDark;
        colors[ImGuiCol_HeaderHovered]          = aneDarkHover;
        colors[ImGuiCol_HeaderActive]           = aneDarkActive;

        colors[ImGuiCol_Separator]              = aneFrame;
        colors[ImGuiCol_SeparatorHovered]       = aneFrameHover;
        colors[ImGuiCol_SeparatorActive]        = aneFrameActive;

        colors[ImGuiCol_ResizeGrip]             = aneColor;
        colors[ImGuiCol_ResizeGripHovered]      = aneColorHover;
        colors[ImGuiCol_ResizeGripActive]       = aneColorActive;
        colors[ImGuiCol_Tab]                    = aneDark;
        colors[ImGuiCol_TabHovered]             = aneDarkHover;
        colors[ImGuiCol_TabActive]              = aneDarkActive;
        colors[ImGuiCol_TabUnfocused]           = ImLerp(imBlack, aneDarkHover, 0.9f);
        colors[ImGuiCol_TabUnfocusedActive]     = ImLerp(imBlack, aneDarkHover, 0.8f);

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

        _aneStyleColor[AneWhite] = Vector4::Convert(aneWhite);
        _aneStyleColor[AneBlack] = Vector4::Convert(aneBlack);
        _aneStyleColor[AneDark] = Vector4::Convert(aneDark);
        _aneStyleColor[AneDarkHover] = Vector4::Convert(aneDarkHover);
        _aneStyleColor[AneDarkActive] = Vector4::Convert(aneDarkActive);
        _aneStyleColor[AneColor] = Vector4::Convert(aneColor);
        _aneStyleColor[AneColorHover] = Vector4::Convert(aneColorHover);
        _aneStyleColor[AneColorActive] = Vector4::Convert(aneColorActive);
        _aneStyleColor[AneHint] = Vector4::Convert(aneHint);
        _aneStyleColor[AneHintHover] = Vector4::Convert(aneHintHover);
        _aneStyleColor[AneHintActive] = Vector4::Convert(aneHintActive);
        _aneStyleColor[AneFrame] = Vector4::Convert(aneFrame);
        _aneStyleColor[AneFrameHover] = Vector4::Convert(aneFrameHover);
        _aneStyleColor[AneFrameActive] = Vector4::Convert(aneFrameActive);

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

    Vector4 ImGuiUtilities::GetAneColor(const AnemoneStyleColors index)
    {
        if(index > AneColorCount) return Vector4::ZeroVector();
        return _aneStyleColor[index];
    }
}
