﻿#include "anepch.h"
#include "CameraController.h"

#include "ANE/Core/Application.h"
#include "ANE/Core/Window.h"
#include "ANE/Core/Scene/Components/CameraComponent.h"
#include "ANE/Core/Scene/Components/TransformComponent.h"
#include "ANE/Events/Event.h"
#include "ANE/Events/EventHandler.h"
#include "ANE/Input/Input.h"
#include "ANE/Input/InputAction.h"
#include "ANE/Math/FMath.h"
#include "ANE/Renderer/Renderer.h"

namespace Engine
{
    void CameraController::OnCreate()
    {
        InputSystem& inputSystem = GetInputSystem();

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

        const ViewportProperties viewport = Application::Get().GetWindow().GetActiveViewportProperties();
        const float w = ToFloat(viewport.Width), h = ToFloat(viewport.Height);

        _cameraComponent->SetPerspective(70.0f, w / h, 0.1f, 10000.f);
        _cameraComponent->SetPriority(-1);
    }

    void CameraController::OnUpdate(float deltaTime)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if((_xInput != 0) || (_yInput != 0) || (_zInput != 0))
        {
            //const float moveSpeed = _isSpeedUp ? 0.05f : 0.01f;
            const float moveSpeed = _flySpeed * .05f;

            //const auto transformMat = _transformComponent->Transform.GetQuaternion();
            //const auto transformMat = (Matrix3x3)_transformComponent->Transform.GetLocalToWorld();
            const auto transform = _transformComponent->Transform;

            const Vector3 right = transform.GetRight() * _xInput;
            const Vector3 up = transform.GetUp() * _yInput;
            const Vector3 forward = transform.GetForward() * _zInput;
            const Vector3 moveVector = (right + up + forward).GetNormalized();
            _transformComponent->Transform.AddPosition(moveVector* moveSpeed);
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
        _pitchRadians = FMath::MirrorClamp(newPitch, FMath::Half_PI);
        _yawRadians += lookSpeed * delta.X;
        _transformComponent->Transform.SetRotation(Vector3{_pitchRadians, _yawRadians, 0});
    }

    void CameraController::OnScroll(Vector2 scrollDelta)
    {
        _flySpeed = FMath::Clamp(_flySpeed + scrollDelta.Y, 0.1f, 20.f);
    }

    void CameraController::OnSpeedup(InputValue inputValue)
    {
        _isSpeedUp = inputValue.GetTriggerState() == TriggerStarted || inputValue.GetTriggerState() == TriggerHolding;
    }

    void CameraController::OnEditorEvent(Event& event)
    {
        if (event.GetEventType() != EventType::WindowResize) return;

        const WindowResizeEvent& resizeEvent = reinterpret_cast<WindowResizeEvent&>(event);
        if(!resizeEvent.IsActiveViewport()) return;
        const float w = ToFloat(resizeEvent.GetWidth()), h = ToFloat(resizeEvent.GetHeight());

        _cameraComponent->SetAspectRatio(w / h);
    }
}


