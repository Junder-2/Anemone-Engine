#pragma once
#include "Collider.h"

namespace Engine
{
    class CapsuleCollider : public Collider
    {
    public:
        CapsuleCollider(rp3d::Collider* collider);

        void SetSize(float radius, float height) const;
        void SetRadius(float radius) const;
        void SetHeight(float height) const;

        float GetRadius() const;
        float GetHeight() const;
    };
}
