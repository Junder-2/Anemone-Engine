#pragma once

#include <entt.hpp>
#include <queue> // todo: we can probably precomile this

#include "InputTypes.h"
#include "MouseInputAction.h"
#include "ANE/Delegate/Delegate.h"

namespace Engine
{
    struct InputValue;
    struct Vector2;
    class Event;
    class InputAction;
    class MouseInputAction;

    /**
    * Class that stores and processes inputs
    */
    class InputHandler
    {
    public:
        static std::unique_ptr<InputHandler> Create();

        InputHandler();
        ~InputHandler();

        void BindOnEvent(const Delegate<void(Event&)>& delegate);
        void OnUpdate();

        /**
        * Registers a keyboard key to be handled and processed
        * @param keyCode id from KeyCodes
        */
        void RegisterKeyboardKey(int keyCode);

        void PopulateKeyStates(const uint8* newKeyStates);
        void ProcessKey(int keyCode, bool press);
        void ProcessMouseMovement(Vector2 pos, Vector2 delta);
        void ProcessAbsoluteMouseMovement(Vector2 pos);
        void ProcessMouseButton(int index, bool press, bool isDoubleClick = false);
        void ProcessMouseScroll(Vector2 delta);
        /**
        * Clears current inputs
        */
        void FlushInputs();

        /**
        * Returns the trigger state of a key. If the requested key isn't registered returns only pressed or not pressed
        */
        TriggerState GetKeyTriggerState(int keyCode);
        /**
        * Returns four currently triggered keys
        */
        // todo: just using for debug now. probably dirty to use array
        std::array<InputValue, 4> GetCurrentTriggeredKeys();

        /**
        * Returns current mouse input data
        */
        MouseInputAction GetMouseInputData() const;

    private:
        void DispatchEvent(Event& e);

    private:
        SinglecastDelegate<void(Event&)> _eventDelegate;

        const Uint8* _currentKeyStates{};
        entt::dense_map<int, InputAction*> _keyboardInputActions {};
        std::queue<int> _dirtyKeys {};
        MouseInputAction _mouseInputAction {};
        bool _dirtyMouse = false;
    };
}
