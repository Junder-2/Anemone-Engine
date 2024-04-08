#pragma once
#include "../ScriptableEntity.h"
#include "ANE/Core/Scene/Components/RenderComponent.h"
#include "ANE/Input/InputSystem.h"

namespace Engine
{
    class CameraController : public ScriptableEntity
    {
    public:

        void OnCreate() override
        {
            InputSystem::Get().BindInput(InputDeviceKeyboard, KeyCodeK, MakeDelegate(this, &CameraController::OnKeyTest));
        }

        void OnDestroy() override // has to be solved
        {

        }

        void OnUpdate(float deltaTime) override
        {

        }

        void OnKeyTest(InputValue inputValue)
        {
            ANE_LOG_INFO("Testing K {}", inputValue.GetIntValue());
        }

    };
}
