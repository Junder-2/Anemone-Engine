#pragma once
#include "Layer.h"
#include "entt.hpp"
#include "ANE/Core/Scene/Scene.h"

namespace Engine
{
    class EditorLayer : public Layer
    {
    public:

        EditorLayer(const std::string& name = "EditorLayer");

        ~EditorLayer() override;

        void OnUIRender() override;
        void DockSpace();

        void Init();

        void OnAttach() override;

        void OnDetach() override;

        void OnEvent(Event& e) override;

        void OnUpdate(float deltaTime) override;

        template <class EntityType>
        void EntityWidget(EntityType& e, entt::basic_registry<EntityType>& reg, bool dropTarget = false);

        std::string GetComponentNameFromEnttId(entt::id_type id);

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

    private:
        void CreateTestScene(int numEntitiesToTest);

        void OnSwitchEditorFocus(InputValue inputValue);

        // static void ShowEditorMenuBar();


    protected:
        std::string _debugName;



    private:
        std::unordered_map<const char*, std::shared_ptr<Scene>> _scenes;
        std::shared_ptr<Scene> _activeScene;
        std::map<entt::id_type, std::string> ComponentTypeMap;

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
