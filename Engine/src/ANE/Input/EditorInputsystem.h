#pragma once
#include "InputSystem.h"

namespace Engine
{
    class EditorInputsystem : public InputSystem
    {
    public:
        EditorInputsystem();

    private:
        void OnEvent(Event& e) override;
    };
}
