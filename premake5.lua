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
        "SPDLOG_USE_STD_FORMAT",
        "GLM_ENABLE_EXPERIMENTAL",
        "GLM_FORCE_DEPTH_ZERO_TO_ONE",
        "GLM_FORCE_LEFT_HANDED"
    }

OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Engine/vendor/imgui"
    include "Engine/vendor/VkBootstrap"
    include "Engine/vendor/Assimp"
    include "Engine/vendor/reactphysics"
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
