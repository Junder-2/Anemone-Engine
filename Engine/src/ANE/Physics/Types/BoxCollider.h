#pragma once
#include "Collider.h"

namespace Engine
{
    class BoxCollider : public Collider
    {
    public:
        BoxCollider(rp3d::Collider* collider);

        void SetHalfSize(Vector3 halfSize) const;
        Vector3 GetHalfSize() const;
    };
}
