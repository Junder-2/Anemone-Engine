#include "anepch.h"
#include "CreateScenePanel.h"
#include "imgui.h"
#include "ANE/Core/Layers/EditorLayer.h"
#include "ANE/Utilities/API.h"

namespace Engine
{
    CreateScenePanel::CreateScenePanel(EditorLayer* layer) : _editorLayer(layer)
    {
        _windowSize = Vector2(WindowXSize, WindowYSize);
        _windowLocation = Vector2((API::WINDOW_SIZE.X * 0.5f) - (WindowXSize * 0.5f), (API::WINDOW_SIZE.Y * 0.5f) - (WindowYSize * 0.5f)); // needs to be total window size
    }

    UIUpdateWrapper CreateScenePanel::OnPanelRender()
    {
        UIUpdateWrapper UIUpdate;

        //ImGui::SetNextWindowPos(_windowLocation);
        ImGui::SetNextWindowSize(_windowSize);
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse;

        bool open;
        ImGui::Begin("Create Scene", &open, flags);

        if (ImGui::InputText("Scene name", _buffer, sizeof(_buffer))) _sceneName = std::string(_buffer);

        if (ImGui::Button("Create", Vector2(60, 20))) // clears the buffer...
        {
            if (!_sceneName.empty())
            {
                open = false;
                CreateScene(_sceneName.c_str());
            }
        }

        ImGui::End();

        if (!open) UIUpdate.RemoveSelf = this;
        return UIUpdate;
    }

    void CreateScenePanel::CreateScene(const char* name) const
    {
        _editorLayer->CreateScene(name);
    }
}
