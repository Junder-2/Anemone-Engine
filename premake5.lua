include "premakeDependencies.lua"

workspace "NP-Engine"
    architecture "x86_64"
    startproject "Sandbox"

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
