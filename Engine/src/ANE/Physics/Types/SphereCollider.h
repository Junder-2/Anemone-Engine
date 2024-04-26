#pragma once
#include "Collider.h"

namespace Engine
{
    class SphereCollider : public Collider
    {
    public:
        SphereCollider(rp3d::Collider* collider);

        void SetRadius(float radius) const;
        float GetRadius() const;
    };
}
