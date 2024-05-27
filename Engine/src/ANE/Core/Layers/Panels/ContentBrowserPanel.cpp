#include "anepch.h"
#include "ContentBrowserPanel.h"

#include "imgui_internal.h"
#include "ANE/Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanRenderer.h"
namespace Engine{

ContentBrowserPanel::ContentBrowserPanel(EditorLayer* editorLayer)
{
    _editorLayer = editorLayer;
    currentDirectory = AssetDirectory;
    Init();
}

void ContentBrowserPanel::Init()
{
    m_AssetIconMap["folder"] = LoadTextureAsImGuiImage("UITextures/UIIcons/FolderIcon.png");
    m_AssetIconMap["cpp"] = LoadTextureAsImGuiImage("UITextures/UIIcons/FolderIcon.png");

}


UIUpdateWrapper ContentBrowserPanel::OnPanelRender()
{
    ANE_DEEP_PROFILE_FUNCTION();

    ImGui::Begin("Content Browser");
    ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;
    if (ImGui::BeginTable("##directoryBrowser", 2, tableFlags, ImVec2(0.0f, 0.0f)))
    {
        //Set up asset browser panel into a basic two column table
        //Column zero has folder navigation
        //Column one renders the contents of folders, along with navigation UI
        ImGui::TableSetupColumn("Outliner", 0, 300.0f);
        ImGui::TableSetupColumn("Directory Structure", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow();

        // Content Outliner
        ImGui::TableSetColumnIndex(0);
        RenderDirectoryBrowserChild(AssetDirectory);

        // Directory Content
        ImGui::TableSetColumnIndex(1);
        RenderDirectoryContentsBrowserChild();

        //End table rendering
        ImGui::EndTable();
    }
    ImGui::End();
    return UILayerPanel::OnPanelRender();
}

void ContentBrowserPanel::RenderDirectoryBrowserChild(std::filesystem::path DirectoryPath)
{
    ANE_DEEP_PROFILE_FUNCTION();

    for (auto const& dir_entry : std::filesystem::directory_iterator{DirectoryPath})
    {
        if (dir_entry.is_directory())
        {
            ImGui::BeginDisabled();
            ImGui::ImageButton(m_AssetIconMap["folder"], _folderIconSize);
            ImGui::EndDisabled();
            ImGui::SameLine();
            bool opened = ImGui::TreeNodeEx(dir_entry.path().filename().string().c_str());
            if (ImGui::IsItemClicked())
            {
                SelectionManager::DeSelect(context);
                SelectionManager::RegisterSelect(context, dir_entry.path().string());
            }
            if (opened)
            {
                RenderDirectoryBrowserChild(dir_entry);
                ImGui::TreePop();
            }
        }
        else
        {
            //Only non folder entries in a directory will end up here. Can potentially do caching and stuff with them
        }
    }
}




void ContentBrowserPanel::RenderDirectoryContentsBrowserChild()
{
    ANE_DEEP_PROFILE_FUNCTION();

    const float topBarHeight = 40.0f;
    const float bottomBarHeight = 32.0f;

    ImGui::BeginChild("##directory_structure", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - topBarHeight - bottomBarHeight));
    {
        RenderDirectoryContentsBrowserChildTopBar(topBarHeight);
        ImGui::Separator();
        {
            //technically should only ever be one folder selected I believe
            std::vector<std::string>* listOfSelectedFilePathAsString = SelectionManager::GetSelection(context);
            if (listOfSelectedFilePathAsString->size() > 0)
            {
                std::string selectedFilePathAsString = listOfSelectedFilePathAsString->at(0);
                std::filesystem::path selectedDirectory = std::filesystem::path(selectedFilePathAsString);
                int columns = ToInt(ImGui::GetContentRegionAvail().x/(_contentGroupSize.x + 25));
                ImGui::Columns(columns, NULL, false);
                for (auto const& dir_entry : std::filesystem::directory_iterator{selectedDirectory})
                {

                    if (!dir_entry.is_directory())
                    {
                        RenderDirectoryItem(dir_entry);
                    }
                }
            }
        }
        ImGui::EndChild();
    }
}
void ContentBrowserPanel::RenderDirectoryItem(std::filesystem::directory_entry dir_entry)
{
    std::string filename = dir_entry.path().filename().string();

    ImTextureID stemIcon;
    std::string extension = dir_entry.path().extension().generic_string();
    if (!extension.empty())
    {
        extension = extension.substr(1, extension.size());
    }
    if (m_AssetIconMap.contains(extension))
    {
        stemIcon = m_AssetIconMap[extension];
    }
    else
    {
        stemIcon = m_AssetIconMap["folder"];
    }
    {
        ImGui::BeginDisabled();
        ImGui::BeginGroup();
        ImGui::ImageButton("Icon",stemIcon,_contentIconSize);
        ImGui::TextWrapped("%s", dir_entry.path().filename().replace_extension("").string().c_str());
        ImGui::EndGroup();
        ImGui::EndDisabled();
        ImGui::NextColumn();
    }
}

VkDescriptorSet ContentBrowserPanel::LoadTextureAsImGuiImage(const std::string& texturePath)
{
    const auto image = Engine::Renderer::LoadTexture("UITextures/UIIcons/FolderIcon.png");
    return ImGui_ImplVulkan_AddTexture(Vulkan::VulkanRenderer::GetNearestSampler(), image.ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void Engine::ContentBrowserPanel::RenderDirectoryContentsBrowserChildTopBar(float topBarHeight)
{
    //All support UI, like searching, navigation and menu's should be rendered in this function
}
} //namespace Engine
