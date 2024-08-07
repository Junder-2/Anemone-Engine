project "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

    files
    {
        "imconfig.h",
        "imgui.cpp",
        "imgui.h",
        "imgui_demo.cpp",
        "imgui_draw.cpp",
        "imgui_internal.h",
        "imgui_stacklayout.h",
        "imgui_stacklayout.cpp",
        "imgui_stacklayout_internal.h",
        "imgui_internal.h",
        "imgui_tables.cpp",
        "imgui_widgets.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
