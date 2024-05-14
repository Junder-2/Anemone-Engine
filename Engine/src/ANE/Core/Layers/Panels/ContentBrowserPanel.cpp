#include "anepch.h"
#include "ContentBrowserPanel.h"
Engine::ContentBrowserPanel::ContentBrowserPanel(EditorLayer* editorLayer)
{
    _editorLayer = editorLayer;
    currentDirectory = AssetDirectory;
}



Engine::UIUpdateWrapper Engine::ContentBrowserPanel::OnPanelRender()
{
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
    bool open = false;
    for (auto const& dir_entry : std::filesystem::directory_iterator{DirectoryPath})
    {
        if (dir_entry.is_directory())
        {
            bool opened = ImGui::TreeNodeEx(dir_entry.path().filename().string().c_str());
            if(ImGui::IsItemClicked())
            {
                SelectionManager::DeSelect(context);
                SelectionManager::RegisterSelect(context,dir_entry.path().string());
            }
            if(opened)
                {
                RenderDirectoryBrowserChild(dir_entry);
                ImGui::TreePop();
                }
        }
        else
        {
            continue;
        }
    }
}


void Engine::ContentBrowserPanel::RenderDirectoryContentsBrowserChild()
{
    const float topBarHeight = 26.0f;
    const float bottomBarHeight = 32.0f;
    ImGui::BeginChild("##directory_structure", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - topBarHeight - bottomBarHeight));
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        RenderDirectoryContentsBrowserChildTopBar(topBarHeight);
        ImGui::PopStyleVar();

        ImGui::Separator();

        ImGui::BeginChild("Scrolling");
        {
            //technically should only ever be one folder selected I believe
            std::vector<std::string>* listOfSelectedFilePathAsString = SelectionManager::GetSelection(context);
            if(listOfSelectedFilePathAsString->size() > 0)
            {
                std::string selectedFilePathAsString = listOfSelectedFilePathAsString->at(0);
                std::filesystem::path selectedDirectory = std::filesystem::path(selectedFilePathAsString);
                for (auto const& dir_entry : std::filesystem::directory_iterator{selectedDirectory})
                {
                    if(dir_entry.is_directory())
                    {
                        continue;
                    }
                    else
                    {
                        ImGui::Text("%s",dir_entry.path().filename().string().c_str());
                        ImTextureID my_tex_id = io.Fonts->TexID;
                        float my_tex_w = (float)io.Fonts->TexWidth;
                        float my_tex_h = (float)io.Fonts->TexHeight;
                        {
                            static bool use_text_color_for_tint = false;
                            ImGui::Checkbox("Use Text Color for Tint", &use_text_color_for_tint);
                            ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
                            ImVec2 pos = ImGui::GetCursorScreenPos();
                            ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
                            ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
                            ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
                            ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
                            ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
                    }
                }
            }
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();
}
void Engine::ContentBrowserPanel::RenderDirectoryContentsBrowserChildTopBar(float topBarHeight)
{

    //All support UI, like searching, navigation and menu's should be rendered in this function

}



