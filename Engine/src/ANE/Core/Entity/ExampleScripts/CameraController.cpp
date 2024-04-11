#include "anepch.h"
#include "CameraController.h"

#include <glm/gtx/quaternion.hpp>

#include "ANE/Renderer/Renderer.h"

namespace Engine
{
    void CameraController::OnCreate()
    {
        InputSystem& inputSystem = GetInputSystem();
        // inputSystem.BindInput(BindingPair(InputDeviceKeyboard, KeyCodeW), MakeDelegate(this, &CameraController::OnKeyTest));
        // inputSystem.BindMouseButton(MouseButtonLeft, MakeDelegate(this, &CameraController::OnKeyTest));
        inputSystem.BindKeyboardAxisInput(KeyCodeA, KeyCodeD, MakeDelegate(this, &CameraController::OnMoveX));
        inputSystem.BindKeyboardAxisInput(KeyCodeQ, KeyCodeE, MakeDelegate(this, &CameraController::OnMoveY));
        inputSystem.BindKeyboardAxisInput(KeyCodeS, KeyCodeW, MakeDelegate(this, &CameraController::OnMoveZ));

        inputSystem.BindMouseMove(MakeDelegate(this, &CameraController::OnLook));

        inputSystem.BindKeyboardInput(KeyCodeLShift, MakeDelegate(this, &CameraController::OnSpeedup));

        _transformComponent = &GetComponent<TransformComponent>();
    }

    void CameraController::OnUpdate(float deltaTime)
    {
        if((_xInput != 0) || (_yInput != 0) || (_zInput != 0))
        {
            const float moveSpeed = _isSpeedUp ? 0.05f : 0.01f;

            const glm::quat orientation = glm::quat(glm::vec3(_pitchRadians, _yawRadians, 0.0f));
            glm::vec3 right = rotate(orientation, glm::vec3{ _xInput, 0, 0 });
            glm::vec3 up = rotate(orientation, glm::vec3{ 0, _yInput, 0 });
            glm::vec3 forward = rotate(orientation, glm::vec3{ 0, 0, _zInput });
            glm::vec3 moveVector = right + up + forward;
            _transformComponent->Transform.AddPosition(moveSpeed * moveVector);
            Renderer::SetCameraPosition(_transformComponent->Transform.GetPosition());
        }
    }

    void CameraController::OnMoveX(InputValue inputValue)
    {
        _xInput = inputValue.GetAxis();
        //ANE_LOG_INFO("Move X = {0} ", _xInput);
    }

    void CameraController::OnMoveY(InputValue inputValue)
    {
        _yInput = inputValue.GetAxis();
        //ANE_LOG_INFO("Move Y = {0} ", _yInput);
    }

    void CameraController::OnMoveZ(InputValue inputValue)
    {
        _zInput = inputValue.GetAxis();
    }

    void CameraController::OnLook(MouseMoveValue mouseValue)
    {
        const glm::vec2 delta = mouseValue.GetMouseDelta();

        constexpr float lookSpeed = 500.0f;
        const float newPitch = _pitchRadians + lookSpeed * delta.y;
        _pitchRadians = glm::clamp(newPitch, -glm::half_pi<float>(), glm::half_pi<float>());
        _yawRadians += lookSpeed * delta.x;
        Renderer::SetCameraRotation(_yawRadians, _pitchRadians);
    }

    void CameraController::OnSpeedup(InputValue inputValue)
    {
        _isSpeedUp = inputValue.GetTriggerState() == TriggerStarted || inputValue.GetTriggerState() == TriggerHolding;
    }
}


