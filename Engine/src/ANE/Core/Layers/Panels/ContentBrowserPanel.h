#pragma once
#include "anepch.h"
#include "imgui.h"
#include <filesystem>

#include "UILayerPanel.h"


namespace Engine
{
    class EditorLayer;

    class ContentBrowserPanel : public UILayerPanel
    {
    public:
        ContentBrowserPanel(EditorLayer* editorLayer);
        ~ContentBrowserPanel()  = default;
        void RenderDirectoryContentsBrowserChildTopBar(float topBarHeight);
        UIUpdateWrapper OnPanelRender() override;

        void RenderDirectoryBrowserChild();
        void RenderDirectoryContentsBrowserChildTopBar();
        void RenderDirectoryContentsBrowserChild();


    private:
        bool _initialized = false;
        EditorLayer* _editorLayer;
        std::filesystem::path currentDirectory;
        //std::map<std::string,ImGui::> m_AssetIconMap;

        inline static const std::filesystem::path AssetDirectory = std::filesystem::path("C:/NP-GameEngine/Assets");
    };
}
