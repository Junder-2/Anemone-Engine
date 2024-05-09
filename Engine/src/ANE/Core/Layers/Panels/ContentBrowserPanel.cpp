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
            RenderDirectoryBrowserChild();

        // Directory Content
            ImGui::TableSetColumnIndex(1);
            RenderDirectoryContentsBrowserChild();

        //End table rendering
            ImGui::EndTable();

        }
        ImGui::End();
        return UILayerPanel::OnPanelRender();
}

void Engine::ContentBrowserPanel::RenderDirectoryBrowserChild()
{
    ImGui::BeginChild("##folders_common");
    {
        //ImGui::BeginChild("##top_bar", ImVec2(0, height));
        //ImGui::BeginHorizontal("##top_bar", ImGui::GetWindowSize());
    }
    ImGui::EndChild();

}
void Engine::ContentBrowserPanel::RenderDirectoryContentsBrowserChildTopBar(float topBarHeight)
{

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
            /*
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.35f));

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
            if (ImGui::BeginPopupContextWindow(0, 1))
            */

        }
        ImGui::EndChild();
    }
    ImGui::EndChild();
}



