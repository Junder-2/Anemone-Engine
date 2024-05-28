-- Engine Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

External = {}
External["SDL2"] = "%{wks.location}/Engine/vendor/SDL2"
External["ImGui"] = "%{wks.location}/Engine/vendor/imgui"
External["ImGuizmo"] = "%{wks.location}/Engine/vendor/ImGuizmo"
External["GLM"] = "%{wks.location}/Engine/vendor/GLM"
External["spdlog"] = "%{wks.location}/Engine/vendor/spdlog"
External["VkBootstrap"] = "%{wks.location}/Engine/vendor/VkBootstrap"
External["entt"] = "%{wks.location}/Engine/vendor/entt"
External["toml"] = "%{wks.location}/Engine/vendor/toml"
External["VMA"] = "%{wks.location}/Engine/vendor/VMA"
External["reactphysics3d"] = "%{wks.location}/Engine/vendor/reactphysics"
External["Assimp"] = "%{wks.location}/Engine/vendor/Assimp"
External["Slang"] = "%{wks.location}/Engine/vendor/Slang"
External["stb"] = "%{wks.location}/Engine/vendor/stb"


-- Include Directories
IncludeDir = {}
IncludeDir["SDL2"] = "%{External.SDL2}/include"
IncludeDir["ImGui"] = "%{External.ImGui}"
IncludeDir["ImGuizmo"] = "%{External.ImGuizmo}"
IncludeDir["GLM"] = "%{External.GLM}"
IncludeDir["spdlog"] = "%{External.spdlog}/include"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["VkBootstrap"] = "%{External.VkBootstrap}/src"
IncludeDir["entt"] = "%{External.entt}/include"
IncludeDir["toml"] = "%{External.toml}/include"
IncludeDir["VMA"] = "%{External.VMA}/include"
IncludeDir["reactphysics3d"] = "%{External.reactphysics3d}/include"
IncludeDir["Assimp"] = "%{External.Assimp}/include"
IncludeDir["Slang"] = "%{External.Slang}"
IncludeDir["stb"] = "%{External.stb}"


-- Library Directories
LibraryDir = {}
LibraryDir["SDL2"] = "%{External.SDL2}/lib/x64"
LibraryDir["Vulkan"] = "%{VULKAN_SDK}/Lib/"
LibraryDir["Slang"] = "%{External.Slang}/lib/x64"
