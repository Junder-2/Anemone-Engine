project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-intermediate/" .. OutputDir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "%{wks.location}/Engine/vendor/imgui/backends/imgui_impl_sdl2.cpp",
        "%{wks.location}/Engine/vendor/imgui/backends/imgui_impl_vulkan.cpp",
    }

    includedirs
    {
        "%{wks.location}/Engine/src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGui}/backends",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.VulkanSDK}",
        "%{IncludeDir.entt}",
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
        defines "NP_RELEASE"
        optimize "on"
        symbols "on"

    filter "configurations:Dist"
        defines "NP_DIST"
        optimize "on"
