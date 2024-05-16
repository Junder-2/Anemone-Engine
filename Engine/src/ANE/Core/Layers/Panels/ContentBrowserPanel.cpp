#include "anepch.h"
#include "ContentBrowserPanel.h"

#include "imgui_internal.h"
#include "ANE/Renderer/Renderer.h"
#include "Platform/Vulkan/VulkanRenderer.h"

Engine::ContentBrowserPanel::ContentBrowserPanel(EditorLayer* editorLayer)
{
    _editorLayer = editorLayer;
    currentDirectory = AssetDirectory;
    Init();
}

void Engine::ContentBrowserPanel::Init()
{
    auto image = Renderer::LoadTexture("UITextures/UIIcons/FolderIcon.png");
    auto icon = ImGui_ImplVulkan_AddTexture(Vulkan::VulkanRenderer::_samplerNearest, image.ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    m_AssetIconMap["folder"] = icon;
    m_AssetIconMap["cpp"] = icon;
}


Engine::UIUpdateWrapper Engine::ContentBrowserPanel::OnPanelRender()
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

void Engine::ContentBrowserPanel::RenderDirectoryBrowserChild(std::filesystem::path DirectoryPath)
{
    ANE_DEEP_PROFILE_FUNCTION();

    bool open = false;
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


bool Engine::ContentBrowserPanel::GetDirEntryIconFromStem(ImTextureID* stemIcon, const std::string& stem)
{
    bool iconPresent = m_AssetIconMap.contains(stem);
    if (iconPresent)
    {
        stemIcon = &m_AssetIconMap[stem];
    }
    return iconPresent;
}

void Engine::ContentBrowserPanel::RenderDirectoryContentsBrowserChild()
{
    const float topBarHeight = 40.0f;
    ANE_DEEP_PROFILE_FUNCTION();

    const float bottomBarHeight = 32.0f;
    ImGui::BeginChild("##directory_structure", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - topBarHeight - bottomBarHeight));
    {
        RenderDirectoryContentsBrowserChildTopBar(topBarHeight);

        ImGui::Separator();

        ImGui::BeginChild("Scrolling");
        {
            //technically should only ever be one folder selected I believe
            std::vector<std::string>* listOfSelectedFilePathAsString = SelectionManager::GetSelection(context);
            if (listOfSelectedFilePathAsString->size() > 0)
            {
                std::string selectedFilePathAsString = listOfSelectedFilePathAsString->at(0);
                std::filesystem::path selectedDirectory = std::filesystem::path(selectedFilePathAsString);
                ImVec2 browserContentIconSize;
                browserContentIconSize.x = floorf(ImGui::GetContentRegionAvail().x / 4);
                browserContentIconSize.y = floorf(ImGui::GetContentRegionAvail().x / 3);
                const ImVec2 item_spacing = ImVec2(20, 20);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(floorf(item_spacing.x), floorf(item_spacing.y)));
                int index = 0;
                int columns = ImGui::GetContentRegionAvail().x /  browserContentIconSize.x ;
                columns = columns < 1 ? 1 : columns;
                float curX = 0.0f;
                ImGui::Columns(columns, nullptr, false);
                ImGui::NextColumn();
                int currColumn = 0;
                for (auto const& dir_entry : std::filesystem::directory_iterator{selectedDirectory})
                {

                    if (!dir_entry.is_directory())
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
                            //ImGui::BeginVertical("IconLabel",ImVec2(50,50));
                            ImGui::BeginDisabled();
                            ImGui::BeginGroup();

                            ImGui::ImageButton(m_AssetIconMap[extension],_contentIconSize);
                            ImGui::TextWrapped("%s", dir_entry.path().filename().replace_extension("").string().c_str());
                            ImGui::EndGroup();

                            ImGui::EndDisabled();

                            //ImGui::EndVertical();
                        }
                        ImGui::NextColumn();

                    }

                }
                ImGui::PopStyleVar();

            }
            ImGui::EndChild();
        }
        ImGui::EndChild();

    }
}
    void Engine::ContentBrowserPanel::RenderDirectoryContentsBrowserChildTopBar(float topBarHeight)
        {
            //All support UI, like searching, navigation and menu's should be rendered in this function
        }
