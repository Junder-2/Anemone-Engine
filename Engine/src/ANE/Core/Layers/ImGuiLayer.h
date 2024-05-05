#pragma once
#include "Layer.h"
#include "entt.hpp"

namespace Engine
{
    struct InputValue;

    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer(const char* layerName);
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float deltaTime) override;
        void OnEvent(Event& e) override;
        static ImGuiLayer* Create(const char* str);
    private:
        void LogWindow(bool* open);
        void SceneViewWindow();
        void HierarchyWindow();
        void OnUIRender() override;
        void DockSpace(bool* open);

        void ShowInputDebugOverlay();
        void MainMenuWindow();
        void FileMenuWindow();
        void CreateSceneWindow();

        bool GetFlag(const char* key);
        void DisableFlag(const char* key);
        void EnableFlag(const char* key);
        void ToggleFlag(const char* key);

        entt::dense_map<std::string, bool> _menuFlags; // should be moved to its own class.
    };
}
