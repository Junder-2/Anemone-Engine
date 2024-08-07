project "Engine"
    kind "StaticLib" --Engine will be DLL
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-intermediate/" .. OutputDir .. "/%{prj.name}")

    pchheader "anepch.h"
    pchsource "src/anepch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/GLM/glm/**.hpp",
        "vendor/GLM/glm/**.inl",
        "vendor/imgui/backends/imgui_impl_sdl2.cpp",
        "vendor/imgui/backends/imgui_impl_vulkan.cpp",
        "vendor/VMA/include/vk_mem_alloc.h",
    }



    includedirs
    {
        "src/",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.SDL2}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGui}/backends",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.toml}",
        "%{IncludeDir.VulkanSDK}",
        "%{IncludeDir.VkBootstrap}",
        "%{IncludeDir.VMA}",
        "%{IncludeDir.reactphysics3d}",
        "%{IncludeDir.Assimp}",
        "%{IncludeDir.Slang}",
        "%{IncludeDir.stb}",
    }

    libdirs
    {
        "%{LibraryDir.SDL2}",
        "%{LibraryDir.Vulkan}",
        "%{LibraryDir.Slang}",
    }

    links
    {
        "SDL2",
        "ImGui",
        "vulkan-1",
        "VkBootstrap",
        "Assimp",
        "slang",
        "reactphysics3d",
    }

    filter "files:vendor/imgui/backends/**.cpp or files:vendor/imgui/backends/**.h"
        flags { "NoPCH" }

    filter "system:windows"
        systemversion "latest"

        links{"Dwmapi.lib"}

        defines
        {
            "ANE_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} \"%{LibraryDir.SDL2}/SDL2.dll\" \"%{wks.location}/bin/" .. OutputDir .. "/Sandbox/\""),
            ("{COPY} \"%{LibraryDir.SDL2}/SDL2.dll\" \"%{wks.location}/bin/" .. OutputDir .. "/Editor/\""),
            ("{COPY} \"%{LibraryDir.Slang}/slang.dll\" \"%{wks.location}/bin/" .. OutputDir .. "/Sandbox/\""),
            ("{COPY} \"%{LibraryDir.Slang}/slang.dll\" \"%{wks.location}/bin/" .. OutputDir .. "/Editor/\""),
        }

    filter "configurations:Debug"
        defines "ANE_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "ANE_RELEASE"
        optimize "on"
        symbols "on"

    filter "configurations:Dist"
        defines "ANE_DIST"
        optimize "on"
        symbols "off"
