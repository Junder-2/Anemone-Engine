#pragma once

namespace Engine
{
    class CollisionListener final : public rp3d::EventListener
    {
    public:
        CollisionListener();
        ~CollisionListener() override;
        void onContact(const CallbackData&) override;
        void onTrigger(const rp3d::OverlapCallback::CallbackData&) override;
    };
}
