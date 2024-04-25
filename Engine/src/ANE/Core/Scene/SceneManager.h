#pragma once

namespace Engine
{
    class Layer;

    class SceneManager
    {
    public:
        static void ChangeScene();

    private:
        std::vector<Layer*> _layers;
    };
}
