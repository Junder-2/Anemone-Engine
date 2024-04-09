#include "anepch.h"
#include "Input.h"

#include "ANE/Core/Application.h"
#include "ANE/Subsystem/SubsystemCollection.h"

Engine::InputSystem& Engine::GetInputSystem()
{
    return *Application::Get().GetSubsystemCollection().GetSubSystem<InputSystem>();
}
