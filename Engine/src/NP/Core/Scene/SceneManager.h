#pragma once

namespace Engine {
    class Layer;

    class SceneManager {
    public:
        static void ChangeScene();

        std::vector<Layer*> _layers;
    };
}
