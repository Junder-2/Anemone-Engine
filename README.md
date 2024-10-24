<picture>
  <source media="(prefers-color-scheme: dark)" srcset="/Resources/Branding/anemone_logo_turquoise_white.png">
  <source media="(prefers-color-scheme: light)" srcset="/Resources/Branding/anemone_logo_turquoise_black.png">
  <img alt="Shows the logo which reads 'ANEMONE engine' and has the 'o' in 'ANEMONE' replaced with a turquoise anemone flower." title="Anemone Engine logo designed by Terese Jerfström" src="/Resources/Branding/anemone_logo_turquoise_black.png">
</picture>

# Anemone Engine [![License: Apache 2.0][Apache 2 Badge]](https://github.com/Junder-2/Anemone-Engine/blob/main/LICENSE)
Anemone is a custom game engine built as the graduation project for Futuregames' Programmer course.
It is designed to support scripting, cross platform rendering, an ECS, and 3D physics and scene management all without requiring a user to write any code.
# Contributors

* [Petter](https://github.com/Junder-2)
* [Nestor](https://github.com/Nestorboy)
* [Gustav Carlberg](https://github.com/Litene)
* [Kyle Sinclair](https://github.com/Kyle-Sinclair?tab=repositories)
# Features

## Performant by Default 
The Anemone Engine has a base game object that is automatically an entity in an ECS. This reduces cache inefficency caused by iterating over hundreds of Game Objects that 
typical OOP design produces without sacrificing the legibility of the codebase.
## UI Exposed Variables
The engine uses the EnTT meta system and the Dear ImGui Library together to provide auto binding of exposed class variables in the inspector. 
Most primitive types and some custom types are already supported and more can easily be added.
## Cross platform Rendering
This C++ engine uses Vulkan, SDL2 and Dear ImGui for its rendering  and input needs.
## Scripting and Physics
The anemone engine supports adding scripts as distinct components and can fully simulate physics and scripts in viewport.


## Libraries
* [SDL2](https://libsdl.org/) [![License: Zlib][Zlib Badge]](https://github.com/libsdl-org/SDL/blob/main/LICENSE.txt)
* [Dear ImGui](https://github.com/ocornut/imgui) [![License: MIT][MIT Badge]](https://github.com/ocornut/imgui/blob/master/LICENSE.txt)
* [spdlog](https://github.com/gabime/spdlog) [![License: MIT][MIT Badge]](https://github.com/gabime/spdlog/blob/v1.x/LICENSE)
* [Vulkan](https://vulkan.lunarg.com/)
* [GLM](https://github.com/g-truc/glm) [![License: Happy Bunny License or MIT][HBL MIT Badge]](https://github.com/g-truc/glm/blob/master/copying.txt)
* [VkBootstrap](https://github.com/charles-lunarg/vk-bootstrap) [![License: MIT][MIT Badge]](https://github.com/charles-lunarg/vk-bootstrap/blob/main/LICENSE.txt)
* [Vulkan Memory Allocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) [![License: MIT][MIT Badge]](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/blob/master/LICENSE.txt)
* [EnTT](https://github.com/skypjack/entt) [![License: MIT][MIT Badge]](https://github.com/skypjack/entt/blob/master/LICENSE)
* [meta](https://github.com/skypjack/meta) [![License: MIT][MIT Badge]](https://github.com/skypjack/meta/blob/master/LICENSE)
* [Assimp](https://github.com/assimp/assimp) [![License: Modified BSD 3-Clause][Modified BSD 3-Clause Badge]](https://github.com/assimp/assimp/blob/master/LICENSE)
* [Slang](https://github.com/shader-slang/slang) [![License: MIT][MIT Badge]](https://github.com/shader-slang/slang/blob/master/LICENSE)
* [toml++](https://marzer.github.io/tomlplusplus/) [![License: MIT][MIT Badge]](https://github.com/marzer/tomlplusplus/blob/master/LICENSE)
* [ReactPhysics3D](https://github.com/DanielChappuis/reactphysics3d) [![License: Zlib][Zlib Badge]](https://github.com/DanielChappuis/reactphysics3d/blob/master/LICENSE)
* [stb](https://github.com/nothings/stb/) [![License: MIT or Public Domain][MIT Public Domain Badge]](https://github.com/nothings/stb/blob/master/LICENSE)

[Apache 2 Badge]: https://img.shields.io/badge/License-Apache_2.0-blue.svg
[HBL MIT Badge]: https://img.shields.io/badge/License-Happy_Bunny_License%20or%20MIT-yellow
[MIT Badge]: https://img.shields.io/badge/License-MIT-yellow.svg
[MIT Public Domain Badge]: https://img.shields.io/badge/License-MIT_or_Public_Domain-yellow
[Modified BSD 3-Clause Badge]: https://img.shields.io/badge/License-Modified_BSD_3--Clause-orange
[Zlib Badge]: https://img.shields.io/badge/License-Zlib-lightgrey.svg
