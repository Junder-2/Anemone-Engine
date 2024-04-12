#include "anepch.h"
#include "Input.h"

#include "ANE/Core/Application.h"
#include "ANE/Subsystem/SubsystemCollection.h"
#include "ANE/Input/EditorInputsystem.h"

namespace Engine
{
    InputSystem& GetInputSystem()
    {
        return *Application::Get().GetSubsystemCollection().GetSubSystem<InputSystem>();
    }

    EditorInputsystem& GetEditorInputSystem()
    {
        return *Application::Get().GetSubsystemCollection().GetSubSystem<EditorInputsystem>();
    }
}


