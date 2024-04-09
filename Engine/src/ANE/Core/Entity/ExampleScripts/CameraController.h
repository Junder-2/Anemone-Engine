#pragma once
#include "../ScriptableEntity.h"
#include "ANE/Input/Input.h"
#include "ANE/Utilities/InputUtilities.h"

namespace Engine
{
    class CameraController : public ScriptableEntity
    {
    public:
        CameraController() = default;

        void OnCreate() override;

        void OnDestroy() override // has to be solved
        {

        }

        void OnUpdate(float deltaTime) override;

        void OnMoveX(InputValue inputValue);

        void OnMoveY(InputValue inputValue);

        void OnKeyTest(InputValue inputValue)
        {
            ANE_LOG_INFO("Testing Button device({0}) id({1}): {2}", InputUtilities::ToString(inputValue.GetDeviceType()), inputValue.GetBindingId(), InputUtilities::ToString(inputValue.GetTriggerState()));
        }

    private:
        TransformComponent* _transformComponent {};
        float _xInput = 0, _yInput = 0;
    };
}
