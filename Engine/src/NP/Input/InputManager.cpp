#include "nppch.h"
#include "InputManager.h"

#include <ranges>

#include "InputAction.h"
#include "InputTypes.h"
#include "../Core/Application.h"

namespace Engine
{
    std::unique_ptr<InputManager> InputManager::Create()
    {
        return std::make_unique<InputManager>();
    }

    InputManager::InputManager() = default;

    InputManager::~InputManager()
    {
        for (const InputAction* val : _keyboardInputActions | std::views::values)
        {
            delete val;
        }

        for (const TwoBindingInput* val : _keyboardTwoBindings | std::views::values)
        {
            delete val;
        }

        _keyboardInputActions.clear();
    }

    void InputManager::RegisterKeyboardTrigger(const int keyCode)
    {
        if (_keyboardInputActions.contains(keyCode)) return;

        InputTrigger* newInputAction = new InputTrigger(keyCode);

        _keyboardInputActions.insert_or_assign(keyCode, newInputAction);
    }

    void InputManager::RegisterKeyboardTwoKeyAxis(const int negativeKeyCode, const int positiveKeyCode)
    {
        const IntPair twoKeys(negativeKeyCode, positiveKeyCode);

        if(_keyboardTwoBindings.contains(twoKeys)) return;

        TwoBindingInput* twoKeyInputBinding = new TwoBindingInput(negativeKeyCode, positiveKeyCode);

        _keyboardTwoBindings.insert_or_assign(twoKeys, twoKeyInputBinding);
    }

    void InputManager::OnUpdate()
    {
        bool needProcessing = false;

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
                needProcessing = false;
                int keyCode = dirtyKeysCopy.front();
                if(_keyboardInputActions[keyCode]->ProcessAction(&needProcessing))
                {
                }
                if(needProcessing)
                {
                    _dirtyKeys.push(keyCode);
                }
                dirtyKeysCopy.pop();
            }
        }
    }

    void InputManager::FlushInputs()
    {
        for (const auto val : _keyboardInputActions | std::views::values)
        {
            if(val->FlushAction())
            {
            }
        }

        _mouseInputAction.FlushAction();
        _currentKeyStates = nullptr;
    }

    void InputManager::PopulateKeyStates(const Uint8* newKeyStates)
    {
        _currentKeyStates = newKeyStates;
    }

    void InputManager::ProcessKey(const int keyCode, const bool press)
    {
        if (!_keyboardInputActions.contains(keyCode)) return;

        bool needProcessing = false;

        if(_keyboardInputActions[keyCode]->PopulateInput((float)(press ? TriggerStarted : TriggerStopped), &needProcessing))
        {
        }

        if(needProcessing)
        {
            _dirtyKeys.push(keyCode);
        }
    }

    void InputManager::ProcessMouseMovement(const float xPos, const float yPos, const float deltaTime)
    {
        _dirtyMouse = _mouseInputAction.PopulateMoveInput(xPos, yPos, deltaTime);
        if(_mouseInputAction.PopulateMoveInput(&_dirtyMouse, xPos, yPos, deltaTime))
        {
        }
    }

    void InputManager::ProcessMouseButton(const int index, const bool press, const bool isDoubleClick /*= false */)
    {
        if(_mouseInputAction.PopulateButtonInput(&_dirtyMouse, index, press ? TriggerStarted : TriggerStopped, isDoubleClick))
        {
        }
    }

    std::array<InputValue, 4> InputManager::GetCurrentTriggeredKeys()
    {
        std::array<InputValue, 4> newArray;

        int index = 0;

        for (const InputAction* val : _keyboardInputActions | std::views::values)
        {
            if (val->GetInputValue().GetIntValue() == 0) continue;

            newArray[index] = val->GetInputValue();
            index++;
            if(index >= 4) break;
        }

        return newArray;
    }

    TriggerState InputManager::GetKeyTriggerState(const int keyCode)
    {
        if (_keyboardInputActions.contains(keyCode))
        {
            return _keyboardInputActions[keyCode]->GetInputValue().GetTriggerState();
        }

        if (_currentKeyStates != nullptr && _currentKeyStates[SDL_GetScancodeFromKey(keyCode)])
        {
            return TriggerHolding;
        }

        //NP_ENGINE_LOG_WARN("No registered of key {0}", keyCode);

        return TriggerNone;
    }
}
