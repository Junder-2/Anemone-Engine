#pragma once

namespace Engine
{
    class PhysicsSystem;

    PhysicsSystem& GetPhysicsSystem();
}

#include "PhysicsSystem.h"
#include "Types/RigidBody.h"
#include "Types/Colliders.h"
