-- Engine Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

External = {}
External["SDL2"] = "%{wks.location}/Engine/vendor/SDL2"
External["ImGui"] = "%{wks.location}/Engine/vendor/imgui"
External["GLM"] = "%{wks.location}/Engine/vendor/GLM"
External["spdlog"] = "%{wks.location}/Engine/vendor/spdlog"
External["VkBootstrap"] = "%{wks.location}/Engine/vendor/VkBootstrap"

-- Include Directories
IncludeDir = {}
IncludeDir["SDL2"] = "%{External.SDL2}/include"
IncludeDir["ImGui"] = "%{External.ImGui}"
IncludeDir["GLM"] = "%{External.GLM}"
IncludeDir["spdlog"] = "%{External.spdlog}/include"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["VkBootstrap"] = "%{External.VkBootstrap}/src"

-- Library Directories
LibraryDir = {}
LibraryDir["SDL2"] = "%{External.SDL2}/lib/x64"
LibraryDir["Vulkan"] = "%{VULKAN_SDK}/Lib/"
