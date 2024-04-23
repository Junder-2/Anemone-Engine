#include "anepch.h"
#include "Physics.h"

#include "ANE/Core/Application.h"
#include "ANE/Subsystem/SubSystemCollection.h"

namespace Engine
{
    PhysicsSystem& Engine::GetPhysicsSystem()
    {
        return *Application::Get().GetSubsystemCollection().GetSubSystem<PhysicsSystem>();
    }
}
