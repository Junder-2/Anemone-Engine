-- Engine Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

External = {}
External["SDL2"] = "%{wks.location}/Engine/vendor/SDL2-2.30.1"
External["ImGui"] = "%{wks.location}/Engine/vendor/imgui-1.90.4"
External["GLM"] = "%{wks.location}/Engine/vendor/GLM"
External["spdlog"] = "%{wks.location}/Engine/vendor/spdlog"

-- Include Directories
IncludeDir = {}
IncludeDir["SDL2"] = "%{External.SDL2}/include"
IncludeDir["ImGui"] = "%{External.ImGui}"
IncludeDir["spdlog"] = "%{External.spdlog}/include"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

-- Library Directories
LibraryDir = {}
LibraryDir["SDL2"] = "%{External.SDL2}/lib/x64"