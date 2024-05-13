#pragma once
#include "Layer.h"
#include "entt.hpp"

namespace Engine
{
    class SceneSerializer;
    class Scene;
    class UILayerPanel;
    struct InputValue;
    struct UIUpdateWrapper;

    class EditorLayer : public Layer
    {
    public:
        EditorLayer(const std::string& name = "EditorLayer");
        ~EditorLayer() override;


        void Init();

        void OnAttach() override;
        void OnDetach() override;

        void OnEvent(Event& e) override;
        void OnUpdate(float deltaTime) override;
        void OnUIRender() override;

        template <class EntityType>
        void EntityWidget(EntityType& e, entt::basic_registry<EntityType>& reg, bool dropTarget = false);

        template <class TValue>
        std::enable_if_t<std::is_base_of_v<UILayerPanel, TValue>> AddPanel(TValue* panel);

        // template <class TValue>
        // std::enable_if_t<std::is_base_of_v<Scene, TValue>> AddScene(const char* key);
        //
        // template <class TValue>
        // std::enable_if_t<std::is_base_of_v<Scene, TValue>> AddScene(const char* key, const std::vector<entt::entity> entities);

        std::shared_ptr<Scene> CreateScene(const char* sceneName) const;
        void SaveScene(InputValue inputValue);
        void SetActiveScene(const char* sceneName);

        std::shared_ptr<Scene> GetActiveScene() { return _activeScene; }

        std::string GetComponentNameFromEnttId(entt::id_type id);

        std::map<entt::id_type, std::string> ComponentTypeMap;

    private:
        SceneSerializer* _sceneSerializer;
        std::vector<UIUpdateWrapper> _uiUpdates;
        void CreateTestScene(int numEntitiesToTest);
        void CreateFloor();

        void OnSwitchEditorFocus(InputValue inputValue);

    private:
        //std::unordered_map<const char*, std::shared_ptr<Scene>> _scenes; // todo: cleanup
        std::shared_ptr<Scene> _activeScene;
        std::map<entt::id_type, std::string> _componentTypeMap;
    };


    // template <class TValue>
    // std::enable_if_t<std::is_base_of_v<Scene, TValue>> EditorLayer::AddScene(const char* key, std::vector<entt::entity> entities)
    // {
    //     std::unique_ptr<TValue> tempScene = std::make_unique<TValue>(entities); // scene creation
    //    // _scenes.emplace(std::make_pair(key, std::move(tempScene)));
    // }
    //
    // template <class TValue>
    // std::enable_if_t<std::is_base_of_v<Scene, TValue>> EditorLayer::AddScene(const char* key)
    // {
    //     std::unique_ptr<TValue> tempScene = std::make_unique<TValue>(std::string(key)); // scene creation
    //     //_scenes.emplace(std::make_pair(key, std::move(tempScene))); // there shouldn't be a scenes dictionary here at all, unnesseray to keep stuff in ram...
    //     if (_activeScene == nullptr)
    //         SetActiveScene(key); //
    // }
    //
    template <class TValue>
    std::enable_if_t<std::is_base_of_v<UILayerPanel, TValue>> EditorLayer::AddPanel(TValue* panel)
    {
        Layer::AttachUIPanel(panel);
    }
}
