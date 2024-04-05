include "premakeDependencies.lua"

workspace "Anemone-Engine"
    architecture "x86_64"
    startproject "Editor"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    defines
    {
        "SPDLOG_USE_STD_FORMAT"
    }

OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Engine/vendor/imgui"
    include "Engine/vendor/VkBootstrap"
group ""

group "Core"
    include "Engine"
group ""

group "Tools"
    include "Editor"
group ""

group "Misc"
    include "Sandbox"
group ""
