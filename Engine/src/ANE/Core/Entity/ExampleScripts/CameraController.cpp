#include "anepch.h"
#include "CameraController.h"

#include <glm/gtx/quaternion.hpp>

#include "ANE/Core/Scene/Components/CameraComponent.h"
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
        _transformComponent->Transform.SetPosition(Vector3{0, 0, -2});

        CameraComponent* camera = &GetComponent<CameraComponent>();
        camera->SetPerspective(70.0f, 900.0f / 500.0f, 10000.f, 0.1f);
    }

    void CameraController::OnUpdate(float deltaTime)
    {
        if((_xInput != 0) || (_yInput != 0) || (_zInput != 0))
        {
            const float moveSpeed = _isSpeedUp ? 0.05f : 0.01f;

            const Matrix3x3 transformMat = (Matrix4x4)_transformComponent->Transform;

            const Vector3 right = transformMat * (Vector3::RightVector() * _xInput);
            const Vector3 up = transformMat * (Vector3::UpVector() * _yInput);
            const Vector3 forward = transformMat * (Vector3::ForwardVector() * _zInput);
            const Vector3 moveVector = (right + up + forward).GetNormalized();
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
        const Vector2 delta = mouseValue.GetMouseDelta();

        constexpr float lookSpeed = 500.0f;
        const float newPitch = _pitchRadians + lookSpeed * delta.Y;
        _pitchRadians = glm::clamp(newPitch, -glm::half_pi<float>(), glm::half_pi<float>());
        Renderer::SetCameraRotation(_yawRadians, _pitchRadians);
        _yawRadians += lookSpeed * delta.X;
    }

    void CameraController::OnSpeedup(InputValue inputValue)
    {
        _isSpeedUp = inputValue.GetTriggerState() == TriggerStarted || inputValue.GetTriggerState() == TriggerHolding;
    }

    Matrix4x4 CameraController::ComputeViewProjMatrix(CameraComponent camera)
    {
        Matrix4x4 transformMatrix = _transformComponent->Transform;
        Matrix4x4 viewMatrix = transformMatrix.GetInverse();
        Matrix4x4 projMatrix = camera.PerspectiveMatrix;
        return projMatrix * viewMatrix;
    }
}


