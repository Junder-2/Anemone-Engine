#pragma once
#include "Collider.h"

namespace Engine
{
    class CapsuleCollider final : public Collider
    {
    public:
        CapsuleCollider(rp3d::Collider* collider);

        void SetSize(float radius, float height);
        void SetRadius(float radius);
        void SetHeight(float height);

        float GetRadius() const;
        float GetHeight() const;

    protected:
        void OnUpdateScale() override;
        float _radius;
        float _height;
    };
}
