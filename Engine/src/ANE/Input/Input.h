#pragma once

namespace Engine
{
    class InputSystem;
    class EditorInputsystem;

    InputSystem& GetInputSystem();
    EditorInputsystem& GetEditorInputSystem();
}

#include "ANE/Input/InputSystem.h"
