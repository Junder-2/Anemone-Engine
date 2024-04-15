#include "anepch.h"
#include "Input.h"

#include "ANE/Core/Application.h"
#include "ANE/Subsystem/SubSystemCollection.h"
#include "ANE/Input/EditorInputSystem.h"

namespace Engine
{
    InputSystem& GetInputSystem()
    {
        return *Application::Get().GetSubsystemCollection().GetSubSystem<InputSystem>();
    }

    EditorInputSystem& GetEditorInputSystem()
    {
        return *Application::Get().GetSubsystemCollection().GetSubSystem<EditorInputSystem>();
    }

    void HideMouse()
    {
        Application::Get().GetWindow().SetMouseVisibility(false);
    }

    void ShowMouse()
    {
        Application::Get().GetWindow().SetMouseVisibility(true);
    }
}


