project "VkBootstrap"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "%{IncludeDir.VulkanSDK}"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
