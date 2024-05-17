#include "anepch.h"
#include "ImGuiAne.h"

namespace Engine::AneImGui
{
    AneImGuiContext* _aneImGui = nullptr;

    AneImGuiContext* Initialize()
    {
        _aneImGui = new AneImGuiContext();
        if(ImGui::GetCurrentContext() == nullptr) ImGui::CreateContext();

        return _aneImGui;
    }

    AneImGuiContext* GetContext()
    {
        return _aneImGui;
    }

    void Cleanup()
    {
        delete _aneImGui;
        ImGui::DestroyContext();
    }

    void EndItem()
    {
        _aneImGui->NextTableDecoratorFlags = 0;
        _aneImGui->NextTableFieldWidth = 0;
    }
}
