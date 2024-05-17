#pragma once
#include "imgui.h"

namespace Engine::AneImGui
{
    struct AneImGuiContext
    {
    };

    extern AneImGuiContext* _aneImGui;

    AneImGuiContext* Initialize();
    AneImGuiContext* GetContext();
    void Cleanup();

    void EndItem();
