-- Engine Dependencies

External = {}
External["SDL2"] = "%{wks.location}/%{prj.name}/vendor/SDL2-2.30.1"
External["ImGui"] = "%{wks.location}/%{prj.name}/vendor/imgui-1.90.4"

-- Include Directories
IncludeDir = {}
IncludeDir["SDL2"] = "%{External.SDL2}/include"
IncludeDir["ImGui"] = "%{External.ImGui}"

-- Library Directories
LibraryDir = {}
LibraryDir["SDL2"] = "%{External.SDL2}/lib/x64"