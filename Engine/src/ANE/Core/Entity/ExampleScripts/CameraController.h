#pragma once
#include "../ScriptableEntity.h"
#include "ANE/Input/Input.h"
#include "ANE/Utilities/InputUtilities.h"

namespace Engine
{
    class CameraController : public ScriptableEntity
    {
    public:
        void OnCreate() override
        {
            InputSystem& inputSystem = GetInputSystem();
            inputSystem.BindInput(BindingPair(InputDeviceKeyboard, KeyCodeW), MakeDelegate(this, &CameraController::OnKeyTest));
            inputSystem.BindMouseButton(MouseButtonLeft, MakeDelegate(this, &CameraController::OnKeyTest));
            inputSystem.BindKeyboardAxisInput(KeyCodeA, KeyCodeD, MakeDelegate(this, &CameraController::OnAxisTest));
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

        void OnAxisTest(InputValue inputValue)
        {
            ANE_LOG_INFO("Testing Axis device({0}) id({1}): {2}", InputUtilities::ToString(inputValue.GetDeviceType()), inputValue.GetBindingId(), inputValue.GetAxis());
        }
    };
}
