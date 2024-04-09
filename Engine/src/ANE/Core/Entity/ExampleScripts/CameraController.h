#pragma once
#include "../ScriptableEntity.h"
#include "ANE/Input/InputSystem.h"
#include "ANE/Utilities/InputUtilities.h"

namespace Engine
{
    class CameraController : public ScriptableEntity
    {
    public:
        void OnCreate() override
        {
            InputSystem& inputSystem = InputUtilities::GetInputSystem();
            inputSystem.BindInput(InputDeviceKeyboard, KeyCodeW, MakeDelegate(this, &CameraController::OnKeyTest));
            inputSystem.BindMouseButton(MouseButtonLeft, MakeDelegate(this, &CameraController::OnKeyTest));
        }

        void OnDestroy() override // has to be solved
        {

        }

        void OnUpdate(float deltaTime) override
        {

        }

        void OnKeyTest(InputValue inputValue)
        {
            ANE_LOG_INFO("Testing Button device({0}) id({1}): {2}", InputUtilities::ToString(inputValue.GetDeviceType()), inputValue.GetBindingId(), InputUtilities::ToString(inputValue.GetTriggerState()));
        }
    };
}
