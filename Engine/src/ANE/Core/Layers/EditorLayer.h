#pragma once
#include "Layer.h"
#include "entt.hpp"
#include "ANE/Core/Scene/Scene.h"

namespace Engine
{
    class EditorLayer : public Layer
    {
    public:
        void OnUIRender() override;

        EditorLayer(const std::string& name = "EditorLayer");

        ~EditorLayer() override;

        void OnAttach() override;

        void OnDetach() override
        {
        }

        void OnEvent(Event& e) override;

        void OnUpdate(float deltaTime) override;

        template <class TValue>
        std::enable_if_t<std::is_base_of_v<Scene, TValue>> AddScene(const char* key);

        template <class TValue>
        std::enable_if_t<std::is_base_of_v<Scene, TValue>> AddScene(const char* key, const std::vector<entt::entity> entities);

        void SetActiveScene(const char* sceneName)
        {
            if (!_scenes.contains(sceneName)) AddScene<Scene>(sceneName);

            _activeScene = _scenes.at(sceneName);
        }

        std::shared_ptr<Scene> GetActiveScene() { return _activeScene; }

    protected:
        std::string _debugName;

    private:
        std::unordered_map<const char*, std::shared_ptr<Scene>> _scenes;
        std::shared_ptr<Scene> _activeScene;
    };

    template <class TValue>
    std::enable_if_t<std::is_base_of_v<Scene, TValue>> EditorLayer::AddScene(const char* key, std::vector<entt::entity> entities)
    {
        std::unique_ptr<TValue> tempScene = std::make_unique<TValue>(entities);
        _scenes.emplace(std::make_pair(key, std::move(tempScene)));
    }

    template <class TValue>
    std::enable_if_t<std::is_base_of_v<Scene, TValue>> EditorLayer::AddScene(const char* key)
    {
        std::unique_ptr<TValue> tempScene = std::make_unique<TValue>();
        _scenes.emplace(std::make_pair(key, std::move(tempScene)));
        if (_scenes.size() == 1)
            SetActiveScene(key);
    }
}
