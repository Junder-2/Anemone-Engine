#pragma once
#include "InputSystem.h"

namespace Engine
{
    class EditorInputSystem final : public InputSystem
    {
    public:
        EditorInputSystem();

    private:
        void OnEvent(Event& e) override;
    };
}
