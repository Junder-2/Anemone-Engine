#pragma once

namespace Engine
{
    class InputSystem;
    class EditorInputsystem;

    InputSystem& GetInputSystem();
    EditorInputsystem& GetEditorInputSystem();

    void HideMouse();
    void ShowMouse();
}

#include "ANE/Input/InputSystem.h"
