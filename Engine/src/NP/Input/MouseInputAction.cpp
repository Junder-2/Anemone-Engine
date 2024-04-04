#include "nppch.h"
#include "MouseInputAction.h"

namespace Engine
{
    MouseInputAction::MouseInputAction() = default;

    MouseInputAction::~MouseInputAction() = default;

    bool MouseInputAction::PopulateMoveInput(bool* needProcessing, const float x, const float y, const float deltaTime)
    {
        const glm::vec2 prevMousePos = _moveValue.GetMousePos();
        _moveValue.SetMousePos(x, y);
        _moveValue.SetMouseDelta((x - prevMousePos.x)*deltaTime, (y - prevMousePos.y)*deltaTime);

        *needProcessing = true;
        return true;
    }

    bool MouseInputAction::PopulateButtonInput(bool* needProcessing, const int buttonIndex, const TriggerState newState, const bool isDoubleClick /*= false */)
    {
        _buttonValue.SetTriggerState(buttonIndex, newState, isDoubleClick);

        *needProcessing = true;
        return true;
    }

    bool MouseInputAction::PopulateScrollInput(bool* needProcessing, const float x, const float y)
    {
        _scrollWheelValue.x = x;
        _scrollWheelValue.y = y;

        *needProcessing = true;
        return true;
    }

    void MouseInputAction::ProcessAction()
    {
        _moveValue.SetMouseDelta(0, 0);
        _scrollWheelValue.x = 0;
        _scrollWheelValue.y = 0;

        for (int i = 0; i < MOUSE_BUTTON_MAX; ++i)
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

    void MouseInputAction::FlushAction()
    {
        _moveValue.SetMouseDelta(0, 0);

        for (int i = 0; i < MOUSE_BUTTON_MAX; ++i)
        {
            auto prevValue = _buttonValue.GetTriggerState(i);
            if(prevValue == TriggerNone) continue;
            _buttonValue.SetTriggerState(i, TriggerNone);
        }
    }
}
