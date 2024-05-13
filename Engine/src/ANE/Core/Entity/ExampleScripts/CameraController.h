#pragma once
#include "ANE/Core/Entity/ScriptableEntity.h"

namespace Engine
{
    struct TransformComponent;
    struct Matrix4x4;
    struct Vector2;
    struct MouseMoveValue;
    struct InputValue;
    struct CameraComponent;

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

        void OnScroll(Vector2 scrollDelta);

        void OnSpeedup(InputValue inputValue);

        void OnEditorEvent(Event& event);

    private:
        TransformComponent* _transformComponent {};
        CameraComponent* _cameraComponent {};
        float _xInput = 0, _yInput = 0, _zInput = 0;
        float _pitchRadians = 0, _yawRadians = 0;
        bool _isSpeedUp;
        float _flySpeed = 1.f;
    };
}
