#include "anepch.h"
#include "CameraController.h"

namespace Engine
{
    void CameraController::OnCreate()
    {
        InputSystem& inputSystem = GetInputSystem();
        // inputSystem.BindInput(BindingPair(InputDeviceKeyboard, KeyCodeW), MakeDelegate(this, &CameraController::OnKeyTest));
        // inputSystem.BindMouseButton(MouseButtonLeft, MakeDelegate(this, &CameraController::OnKeyTest));
        inputSystem.BindKeyboardAxisInput(KeyCodeA, KeyCodeD, MakeDelegate(this, &CameraController::OnMoveX));
        inputSystem.BindKeyboardAxisInput(KeyCodeS, KeyCodeW, MakeDelegate(this, &CameraController::OnMoveY));

        _transformComponent = &GetComponent<TransformComponent>();
    }

    void CameraController::OnUpdate(float deltaTime)
    {
        if(_yInput != 0 || _xInput != 0)
        {
            _transformComponent->Transform.AddPosition(glm::vec3(_xInput*deltaTime, _yInput*deltaTime, 0));
            ANE_LOG_INFO("Pos = {0} ", glm::to_string(_transformComponent->Transform.GetPosition()));
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
}


