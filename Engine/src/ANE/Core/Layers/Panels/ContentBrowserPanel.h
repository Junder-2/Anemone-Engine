#pragma once
#include "anepch.h"
#include "imgui.h"
#include <filesystem>

#include "UILayerPanel.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "Platform/Vulkan/VmaTypes.h"


namespace Engine
{
    class EditorLayer;

    class ContentBrowserPanel : public UILayerPanel
    {
    public:
        ContentBrowserPanel(EditorLayer* editorLayer);
        void Init();
        ~ContentBrowserPanel()  = default;
        void RenderDirectoryContentsBrowserChildTopBar(float topBarHeight);
        UIUpdateWrapper OnPanelRender() override;

        void RenderDirectoryBrowserChild(std::filesystem::path DirectoryPath);
        void RenderDirectoryContentsBrowserChildTopBar();
        void RenderDirectoryContentsBrowserChild();
        void RenderDirectoryItem(std::filesystem::directory_entry);
        static VkDescriptorSet LoadTextureAsImGuiImage(const std::string& texturePath);


    private:
        bool _initialized = false;
        SelectionManager::SelectionContext context = SelectionManager::AssetBrowser;
        EditorLayer* _editorLayer;
        std::filesystem::path currentDirectory;
        std::map<std::string,ImTextureID> m_AssetIconMap;
        ImVec2 _folderIconSize = ImVec2 (12,12);
        ImVec2 _contentIconSize = ImVec2 (120,120);
        ImVec2 _contentGroupSize = ImVec2 (120,200);

        inline static const std::filesystem::path AssetDirectory = std::filesystem::path("../Assets/");
    };
}
