#pragma once
#include "InputTypes.h"
#include "ANE/Core/Math/Vector2.h"
#include "glm/vec2.hpp"

namespace Engine
{
    /**
    * Stores the mouse position and deltas
    */
    struct MouseMoveValue
    {
    public:
        void SetMousePos(const float x, const float y)
        {
            _mousePosition.X = x;
            _mousePosition.Y = y;
        }

        void SetMouseDelta(const float x, const float y)
        {
            _mouseDelta.X = x;
            _mouseDelta.Y = y;
        }

        Vector2 GetMousePos() const { return _mousePosition; }
        Vector2 GetMouseDelta() const { return _mouseDelta; }

    private:
        Vector2 _mouseDelta = {};
        Vector2 _mousePosition = {};
    };

    /**
    * Stores the mouse buttons state
    */
    struct MouseButtonValues
    {
        /**
        * Gets the button index of the latest press
        */
        int GetCurrentButtonIndex() const
        {
            return _lastIndex;
        }

        /**
        * If latest press is double click
        */
        bool GetIsDoubleClick() const
        {
            return _isDoubleClick;
        }

        /**
        * Gets the bitfield of the button values (each button is two bits)
        */
        uint16_t GetRawButtonStates() const
        {
            return _buttonStates;
        }

        /**
        * Gets the trigger state of latest press
        */
        TriggerState GetTriggerState() const
        {
            return GetTriggerStateFromMouseButtonState(_buttonStates, _lastIndex);
        }

        TriggerState GetTriggerState(const int index) const
        {
            return GetTriggerStateFromMouseButtonState(_buttonStates, index);
        }

        void SetTriggerState(const int index, const TriggerState newState, const bool isDoubleClick = false)
        {
            _isDoubleClick = isDoubleClick;
            _lastIndex = std::min(index, MOUSE_BUTTON_MAX-1);
            // Clear and add new state to button
            _buttonStates &= ~GetMouseButtonMaskFromMouseButton(index);
            _buttonStates |= GetMouseButtonStateFromMouseButton(newState, index);
        }

    private:
        int _lastIndex = 0;
        bool _isDoubleClick = false;
        /**
        * each button is two bits
        */
        uint16_t _buttonStates = 0;
    };

    /**
    * Processes mouse inputs
    */
    class MouseInputAction
    {
    public:
        MouseInputAction();
        ~MouseInputAction();

        /**
        * Populates new move input
        * @param needProcessing if this action needs processing next frame
        * @returns stored data changed
        */
        bool PopulateMoveInput(bool* needProcessing, float x, float y, float deltaTime);
        /**
        * Populates new mouse button input
        * @param needProcessing if this action needs processing next frame
        * @param buttonIndex the mouse button to populate
        * @returns stored data changed
        */
        bool PopulateButtonInput(bool* needProcessing, int buttonIndex, TriggerState newState, bool isDoubleClick = false);
        /**
        * Populates new mouse scroll input
        * @param needProcessing if this action needs processing next frame
        * @returns stored data changed
        */
        bool PopulateScrollInput(bool* needProcessing, float x, float y);
        /**
        * Processes actions by clearing deltas
        * * @returns stored data changed
        */
        bool ProcessAction();
        /**
        * Clears input data
        * @returns should send forward
        */
        bool FlushAction();

        MouseMoveValue GetMoveValue() const { return _moveValue; }
        MouseButtonValues GetButtonValue() const { return _buttonValue; }
        Vector2 GetScrollValue() const { return _scrollWheelValue; }

    protected:
        MouseMoveValue _moveValue {};
        MouseButtonValues _buttonValue {};
        Vector2 _scrollWheelValue {};
    };
}
