include "premakeDependencies.lua"

workspace "NP-Engine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Engine/vendor/imgui-1.90.4"
group ""

project "Engine"
    location "Engine"
    kind "SharedLib" --Engine will be DLL
    language "C++"
    cppdialect "C++20"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.ImGui}"
        "%{prj.name}/vendor/spdlog/include"
    }

    libdirs
    {
        "%{LibraryDir.SDL2}"
    }

    links
    {
        "SDL2",
        "ImGui"
    }

    filter "system:windows"
        staticruntime "on"
        systemversion "latest"

        defines
        {
            "NP_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\""),
            ("{COPY} \"%{LibraryDir.SDL2}/SDL2.dll\" \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "NP_DEBUG"
        symbols "on"

    filter "configurations:Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.ImGui}",
        "%{wks.location}/Engine/vendor/spdlog/include",
        "Engine/src"
    }

    links
    {
        "Engine"
    }

    filter "system:windows"
        staticruntime "on"
        systemversion "latest"

    filter "configurations:Debug"
        defines "NP_DEBUG"
        symbols "on"

    filter "configurations:Release"
        optimize "on"