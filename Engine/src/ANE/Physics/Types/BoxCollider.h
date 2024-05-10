#pragma once
#include "Collider.h"

namespace Engine
{
    class BoxCollider final : public Collider
    {
    public:
        BoxCollider(rp3d::Collider* collider);

        void SetHalfSize(Vector3 halfSize);
        Vector3 GetHalfSize() const;

    protected:
        void OnUpdateScale() override;
        Vector3 _halfSize;
    };
}
