#pragma once

namespace Engine
{
    class Scene;

    class SceneBuilder
    {
    public:
        static std::shared_ptr<Scene> BuildScene(std::string sceneName);
    };
}
