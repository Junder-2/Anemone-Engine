project "Sandbox"
    kind "ConsoleApp"
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
        "%{wks.location}/Engine/vendor/spdlog/include",
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.GLM}",
        "%{wks.location}/Engine/src"
    }

    links
    {
        "Engine"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "NP_DEBUG"
        symbols "on"

    filter "configurations:Release"
        optimize "on"