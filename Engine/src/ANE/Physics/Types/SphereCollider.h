#pragma once
#include "Collider.h"

namespace Engine
{
    class SphereCollider final : public Collider
    {
    public:
        SphereCollider(rp3d::Collider* collider);

        void SetRadius(float radius);
        float GetRadius() const;

    protected:
        void OnUpdateScale() override;
        float _radius;
    };
}
