#include "anepch.h"
#include "TransformMatrix.h"

#include "ANE/Math/VMath.h"
#include "ANE/Physics/PhysicsTypes.h"

namespace Engine
{
    void TransformMatrix::MakeScaleSafe()
    {
        _scale = Math::MaxOrMin(_scale, MIN_SCALE);
    }
}
