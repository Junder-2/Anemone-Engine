#include "anepch.h"
#include "CameraController.h"

#include <glm/gtx/quaternion.hpp>

#include "ANE/Core/Application.h"
#include "ANE/Core/Scene/Components/CameraComponent.h"
#include "ANE/Events/EventHandler.h"
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
        inputSystem.BindMouseScroll(MakeDelegate(this, &CameraController::OnScroll));

        inputSystem.BindKeyboardInput(KeyCodeLShift, MakeDelegate(this, &CameraController::OnSpeedup));

        EventHandler::BindEditorEvent(MakeDelegate(this, &CameraController::OnEditorEvent));

        _transformComponent = &GetComponent<TransformComponent>();
        _transformComponent->Transform.SetPosition(Vector3{0, 0, -5});

        _cameraComponent = &GetComponent<CameraComponent>();

        const WindowProperties windowProps = Application::Get().GetWindow().GetProperties();
        const float w = (float)windowProps.Width, h = (float)windowProps.Height;

        _cameraComponent->SetPerspective(70.0f, w / h, 10000.f, 0.1f);
        Renderer::SetViewProjection(ComputeViewProjMatrix(*_cameraComponent));
    }

    void CameraController::OnUpdate(float deltaTime)
    {
        if((_xInput != 0) || (_yInput != 0) || (_zInput != 0))
        {

            //const float moveSpeed = _isSpeedUp ? 0.05f : 0.01f;
            const float moveSpeed = _flySpeed * .05f;

            //const auto transformMat = _transformComponent->Transform.GetQuaternion();
            //const auto transformMat = (Matrix3x3)_transformComponent->Transform.GetLocalToWorld();
            auto transform = _transformComponent->Transform;

            const Vector3 right = transform.GetRight() * _xInput;
            const Vector3 up = transform.GetUp() * _yInput;
            const Vector3 forward = transform.GetForward() * _zInput;
            const Vector3 moveVector = (right + up + forward).GetNormalized();
            _transformComponent->Transform.AddPosition(moveVector* moveSpeed);

            if (_cameraComponent)
            {
                Renderer::SetViewProjection(ComputeViewProjMatrix(*_cameraComponent));
            }
        }
    }

    void CameraController::OnFixedUpdate(float timeStep)
    {

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

        constexpr float lookSpeed = (180.f * FMath::DEGREES_TO_RAD);
        const float newPitch = _pitchRadians + lookSpeed * delta.Y;
        _pitchRadians = glm::clamp(newPitch, -FMath::Half_PI, FMath::Half_PI);
        _yawRadians += lookSpeed * delta.X;
        _transformComponent->Transform.SetRotation(Vector3{_pitchRadians, _yawRadians, 0});

        if (_cameraComponent)
        {
            Renderer::SetViewProjection(ComputeViewProjMatrix(*_cameraComponent));
        }
    }

    void CameraController::OnScroll(Vector2 scrollDelta)
    {
        _flySpeed = glm::clamp(_flySpeed + scrollDelta.Y, 0.1f, 20.f);
    }

    void CameraController::OnSpeedup(InputValue inputValue)
    {
        _isSpeedUp = inputValue.GetTriggerState() == TriggerStarted || inputValue.GetTriggerState() == TriggerHolding;
    }

    void CameraController::OnEditorEvent(Event& event)
    {
        if (event.GetEventType() != EventType::WindowResize) return;

        const WindowResizeEvent& resizeEvent = dynamic_cast<WindowResizeEvent&>(event);
        const float w = (float)resizeEvent.GetWidth(), h = (float)resizeEvent.GetHeight();

        _cameraComponent->UpdateAspectRatio(w / h);
        Renderer::SetViewProjection(ComputeViewProjMatrix(*_cameraComponent));
    }

    Matrix4x4 CameraController::ComputeViewProjMatrix(CameraComponent camera)
    {
        Matrix4x4 transformMatrix = _transformComponent->Transform.GetWorldToLocal();
        Matrix4x4 viewMatrix = transformMatrix;
        Matrix4x4 projMatrix = camera.PerspectiveMatrix;
        return projMatrix * viewMatrix;
    }
}


