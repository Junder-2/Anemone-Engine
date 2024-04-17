#pragma once

namespace Engine
{
    class InputSystem;
    class EditorInputSystem;

    /**
    * Returns the editor Inputsystem (This takes priority over app inputsystem)
    */
    EditorInputSystem& GetEditorInputSystem();
    /**
    * Returns the app Inputsystem
    */
    InputSystem& GetInputSystem();

    /**
    * Hides mouse/constrains it to the window
    */
    void HideMouse();
    /**
    * Shows mouse/allows mouse to exit window
    */
    void ShowMouse();

    /**
    * Is mouse Visible
    */
    bool IsMouseVisible();
}

#include "ANE/Input/InputSystem.h"
