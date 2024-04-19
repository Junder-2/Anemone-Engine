#pragma once
#include "ANE/Core/Entity/ScriptableEntity.h"
#include "ANE/Core/Scene/Components/CameraComponent.h"
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

        void OnFixedUpdate(float timeStep) override;

        void OnMoveX(InputValue inputValue);

        void OnMoveY(InputValue inputValue);

        void OnMoveZ(InputValue inputValue);

        void OnLook(MouseMoveValue mouseValue);

        void OnSpeedup(InputValue inputValue);

        void OnKeyTest(InputValue inputValue)
        {
            ANE_LOG_INFO("Testing Button device({0}) id({1}): {2}", InputUtilities::ToString(inputValue.GetDeviceType()), inputValue.GetBindingId(), InputUtilities::ToString(inputValue.GetTriggerState()));
        }

        void OnEditorEvent(Event& event);

        Matrix4x4 ComputeViewProjMatrix(CameraComponent camera);

    private:
        TransformComponent* _transformComponent {};
        CameraComponent* _cameraComponent {};
        float _xInput = 0, _yInput = 0, _zInput = 0;
        float _pitchRadians = 0, _yawRadians = 0;
        bool _isSpeedUp;
    };
}
