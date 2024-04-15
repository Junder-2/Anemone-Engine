#pragma once

namespace Engine
{
    class InputSystem;
    class EditorInputSystem;

    EditorInputSystem& GetEditorInputSystem();
    InputSystem& GetInputSystem();

    void HideMouse();
    void ShowMouse();
}

#include "ANE/Input/InputSystem.h"
