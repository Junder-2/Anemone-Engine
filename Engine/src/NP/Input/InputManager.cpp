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

    void InputManager::RegisterKeyboardTrigger(const int keyCode)
    {
        if (_keyboardInputActions.contains(keyCode)) return;

        InputTrigger* newInputAction = new InputTrigger();

        _keyboardInputActions.insert_or_assign(keyCode, newInputAction);
    }

    void InputManager::OnUpdate()
    {
        if(_dirtyMouse)
        {
            _mouseInputAction.ProcessAction();
        }

        if(!_dirtyKeys.empty())
        {
            std::queue<int> dirtyKeysCopy;
            std::swap(_dirtyKeys, dirtyKeysCopy);

            while(!dirtyKeysCopy.empty())
            {
                int keyCode = dirtyKeysCopy.front();
                if(_keyboardInputActions[keyCode]->ProcessAction())
                {
                    _dirtyKeys.push(keyCode);
                }
                dirtyKeysCopy.pop();
            }
        }
    }

    void InputManager::ProcessKey(const int keyCode, const bool press)
    {
        if (!_keyboardInputActions.contains(keyCode)) return;

        if(_keyboardInputActions[keyCode]->PopulateInput(press ? TriggerStarted : TriggerStopped))
        {
            _dirtyKeys.push(keyCode);
        }
    }

    void InputManager::ProcessMouseMovement(const float xPos, const float yPos, const float deltaTime)
    {
        _dirtyMouse = _mouseInputAction.PopulateMoveInput(xPos, yPos, deltaTime);
    }

    void InputManager::ProcessMouseButton(const int index, const bool press)
    {
        _dirtyMouse = _mouseInputAction.PopulateButtonInput(index, press ? TriggerStarted : TriggerStopped);
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
