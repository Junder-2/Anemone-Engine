#pragma once
#include "entt.hpp"
#include "ANE/Core/Scene/Scene.h"
#include "ANE/Core/Layers/Layer.h"

namespace SandBox
{
    class SandBoxLayer : public Engine::Layer
    {
    public:
        ~SandBoxLayer() override;
        void Init();
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float deltaTime) override;
        void OnEvent(Engine::Event& e) override;
        void OnUIRender() override;
        template <class TValue>
        std::enable_if_t<std::is_base_of_v<Engine::Scene, TValue>> OpenScene(const char* key);


    private:
        std::unordered_map<const char*, std::shared_ptr<Engine::Scene>> _scenes;
        std::shared_ptr<Engine::Scene> _activeScene;
        //Engine::SceneSerializer* _sceneSerializer;

    };

    template <class TValue>
         std::enable_if_t<std::is_base_of_v<Engine::Scene, TValue>> SandBoxLayer::OpenScene(const char* key)
    {
        std::unique_ptr<TValue> tempScene = std::make_unique<TValue>();
        _scenes.emplace(std::make_pair(key, std::move(tempScene)));
    }

}
