project "Engine"
    kind "StaticLib" --Engine will be DLL
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-intermediate/" .. outputdir .. "/%{prj.name}")

    pchheader "nppch.h"
    pchsource "src/nppch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/GLM/glm/**.hpp",
        "vendor/GLM/glm/**.inl",
        "vendor/imgui/backends/imgui_impl_sdl2.cpp",
        "vendor/imgui/backends/imgui_impl_vulkan.cpp",
    }

    includedirs
    {
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGui}/backends",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.VulkanSDK}",
    }

    libdirs
    {
        "%{LibraryDir.SDL2}",
        "%{LibraryDir.Vulkan}",
    }

    links
    {
        "SDL2",
        "ImGui",
        "vulkan-1"
    }

    filter "files:vendor/imgui/backends/**.cpp or files:vendor/imgui/backends/**.h"
        flags { "NoPCH" }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "NP_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} \"%{LibraryDir.SDL2}/SDL2.dll\" \"%{wks.location}/bin/" .. outputdir .. "/Sandbox/\""),
            ("{COPY} \"%{LibraryDir.SDL2}/SDL2.dll\" \"%{wks.location}/bin/" .. outputdir .. "/Editor/\"")
        }

    filter "configurations:Debug"
        defines "NP_DEBUG"
        symbols "on"

    filter "configurations:Release"
        optimize "on"
        symbols "on"

    filter "configurations:Dist"
        optimize "on"
        symbols "off"
