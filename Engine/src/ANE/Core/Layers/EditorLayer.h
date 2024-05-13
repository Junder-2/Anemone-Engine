#pragma once
#include "Layer.h"
#include "entt.hpp"
#include "ANE/Core/Entity/Entity.h"
#include "ANE/Core/Scene/Scene.h"
#include "ANE/Utilities/SceneSerializer.h"
#include "Panels/UIUpdateWrapper.h"

namespace Engine
{
    struct InputValue;
    class UILayerPanel;

    class EditorLayer : public Layer
    {
    public:
        EditorLayer(const std::string& name = "EditorLayer");

        ~EditorLayer() override;

        void OnUIRender() override;
        //void DockSpace();

        void Init();

        void OnAttach() override;

        void OnDetach() override;

        void OnEvent(Event& e) override;

        void OnUpdate(float deltaTime) override;

        template <class EntityType>
        void EntityWidget(EntityType& e, entt::basic_registry<EntityType>& reg, bool dropTarget = false);

        std::string GetComponentNameFromEnttId(entt::id_type id);

        template <class TValue>
        std::enable_if_t<std::is_base_of_v<UILayerPanel, TValue>> AddPanel(TValue* panel);
        template <class TValue>
        std::enable_if_t<std::is_base_of_v<Scene, TValue>> AddScene(const char* key);

        template <class TValue>
        std::enable_if_t<std::is_base_of_v<Scene, TValue>> AddScene(const char* key, const std::vector<entt::entity> entities);

        void SetActiveScene(const char* sceneName)
        {
            if (!_scenes.contains(sceneName)) AddScene<Scene>(sceneName);

            _sceneSerializer = new SceneSerializer(_activeScene);
            _activeScene = _scenes.at(sceneName);
        }

        std::shared_ptr<Scene> GetActiveScene() { return _activeScene; }

        Entity Create(const char* name = "Untagged");
        Entity Create(std::string = "Untagged");
        Entity GetEntityWithUUID(std::string UUID);

        std::map<entt::id_type, std::string> ComponentTypeMap;

    private:
        SceneSerializer* _sceneSerializer;
        std::map<std::string, Entity> _entityMap;
        std::vector<UIUpdateWrapper> UIUpdates;
        void CreateTestScene(int numEntitiesToTest);
        void CreateFloor();

        void OnSwitchEditorFocus(InputValue inputValue);


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

    template <class TValue>
    std::enable_if_t<std::is_base_of_v<UILayerPanel, TValue>> EditorLayer::AddPanel(TValue* panel)
    {
        Layer::AttachUIPanel(panel);
    }
}
