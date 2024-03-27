#include "nppch.h"
#include "MouseInputAction.h"

namespace Engine
{
    MouseInputAction::MouseInputAction() = default;

    MouseInputAction::~MouseInputAction()
    {
        _buttonInputDelegate.Clear();
        _moveInputDelegate.Clear();
    }

    bool MouseInputAction::PopulateMoveInput(const float x, const float y, const float deltaTime)
    {
        const glm::vec2 prevMousePos = _moveValue.GetMousePos();
        _moveValue.SetMousePos(x, y);
        _moveValue.SetMouseDelta((x - prevMousePos.x)*deltaTime, (y - prevMousePos.y)*deltaTime);

        if(_moveInputDelegate) _moveInputDelegate(_moveValue);

        return true;
    }

    bool MouseInputAction::PopulateButtonInput(const int buttonIndex, const TriggerState newState)
    {
        _buttonValue.SetTriggerState(buttonIndex, newState);

        if(_buttonInputDelegate) _buttonInputDelegate(_buttonValue);

        return true;
    }

    void MouseInputAction::ProcessAction()
    {
        _moveValue.SetMouseDelta(0, 0);

        for (int i = 0; i < MouseButtonValue::indexLength; ++i)
        {
            switch (_buttonValue.GetTriggerState(i))
            {
            case TriggerStarted:
                _buttonValue.SetTriggerState(i, TriggerHolding);
                break;
            case TriggerStopped:
                _buttonValue.SetTriggerState(i, TriggerNone);
            }
        }
    }
}
