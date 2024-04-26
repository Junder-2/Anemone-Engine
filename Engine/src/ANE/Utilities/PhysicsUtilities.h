#pragma once
#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    namespace PhysicsUtilities
    {
        inline std::string ToString(const CollisionShapeType type)
        {
            switch (type)
            {
                case CollisionShapeType::Box: return "Box";
                case CollisionShapeType::Sphere: return "Sphere";
                case CollisionShapeType::Capsule: return "Capsule";
                default: return "Unknown";
            }
        }
    }
}
