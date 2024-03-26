#include "nppch.h"
#include "InputManager.h"

#include "InputAction.h"
#include "InputTypes.h"

namespace Engine
{
    std::unique_ptr<InputManager> InputManager::Create()
    {
        return std::make_unique<InputManager>();
    }

    InputManager::InputManager() = default;

    InputManager::~InputManager()
    {
        for (auto it = _keyboardInputActions.begin(); it != _keyboardInputActions.end(); ++it)
        {
            delete it->second;
        }

        _keyboardInputActions.clear();
    }

    void InputManager::OnUpdate()
    {
        for (auto it = _keyboardInputActions.begin(); it != _keyboardInputActions.end(); ++it)
        {
            it->second->UpdateAction();
        }
    }

    void InputManager::ProcessKey(const int keyCode, const bool press)
    {
        if (_keyboardInputActions.contains(keyCode))
        {
            _keyboardInputActions[keyCode]->PopulateInput(press ? TriggerStarted : TriggerStopped);
        }
    }

    void InputManager::ProcessMouseMovement(const float xPos, const float yPos, const float deltaTime)
    {
        _mouseInputAction.PopulateMoveInput(xPos, yPos, deltaTime);
    }

    void InputManager::ProcessMouseButton(const int index, const bool press)
    {
        _mouseInputAction.PopulateButtonInput(index, press ? TriggerStarted : TriggerStopped);
    }

    TriggerState InputManager::GetKeyTriggerState(int keyCode)
    {
        if (_keyboardInputActions.contains(keyCode))
        {
            return _keyboardInputActions[keyCode]->GetInputValue().GetTriggerState();
        }

        NP_ENGINE_LOG_WARN("No registered of key {0}", keyCode);

        return TriggerNone;
    }
}
