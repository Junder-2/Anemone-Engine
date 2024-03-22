project "Engine"
    location "Engine"
    kind "SharedLib" --Engine will be DLL
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-intermediate/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "vendor/spdlog/include",
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.ImGui}",
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
        systemversion "latest"

        defines
        {
            "NP_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"%{wks.location}/bin/" .. outputdir .. "/Sandbox/\""),
            ("{COPY} \"%{LibraryDir.SDL2}/SDL2.dll\" \"%{wks.location}/bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "NP_DEBUG"
        symbols "on"

    filter "configurations:Release"
        optimize "on"