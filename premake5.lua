include "premakeDependencies.lua"

workspace "NP-Engine"
    architecture "x64"

    defines
    {
        "SPDLOG_USE_STD_FORMAT"
    }

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Engine/vendor/imgui"
group ""

group "Core"
    include "Engine"
group ""

group "Misc"
    include "Sandbox"
group ""