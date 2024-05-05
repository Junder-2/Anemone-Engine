#pragma once
#include <SDL_keycode.h>
#include <functional> //This may be needed for compilatino of these types outside of Engine
#include "ANE/Utilities/CommonUtilities.h"

namespace Engine
{
    /**
    * Type of input source, used to determine the derived class
    */
    typedef enum : uint8_t
    {
        InputDeviceUndefined,
        InputDeviceKeyboard,
        InputDeviceGamepad,
        InputDeviceMouse,
    } InputDeviceType;

    /**
    * Type of input value, used to determine the derived class
    */
    typedef enum : uint8_t
    {
        InputTypeBoolean,
        InputTypeTrigger,
        InputTypeAxis
    } InputValueType;

    /**
    * The trigger state/stage
    */
    typedef enum
    {
        TriggerNone,
        TriggerStarted,
        TriggerHolding,
        TriggerStopped,
    } TriggerState;

    /**
    * Structure holding InputDeviceType and bindingId (KeyCodes, MouseButton)
    */
    struct BindingPair
    {
        BindingPair(const int deviceType, const int bindingId) : DeviceType(deviceType), BindingId(bindingId) {}
        BindingPair(const InputDeviceType deviceType, const int bindingId) : DeviceType(deviceType), BindingId(bindingId) {}
        int DeviceType;
        int BindingId;

        bool operator<(const BindingPair& rhs) const { return DeviceType != rhs.DeviceType ? DeviceType < rhs.DeviceType : BindingId < rhs.BindingId; } // for ordered map
        bool operator==(const BindingPair& rhs) const { return DeviceType == rhs.DeviceType && BindingId == rhs.BindingId;} // for hash
    };

    /**
    * Structure a unique id and the sign of the axis output. Used to link inputs to an axis
    */
    struct AxisBinding
    {
        AxisBinding(const int id = -1, const bool sign = false) : Id(id), Sign(sign) {}

        int Id;
        bool Sign;
    };

    /**
    * The mouse button index
    */
    typedef enum
    {
        MouseButtonLeft = 0,
        MouseButtonMiddle = 1,
        MouseButtonRight = 2,
        MouseButtonX1 = 3,
        MouseButtonX2 = 4,
    } MouseButton;

    #define MOUSE_BUTTON_MAX (MouseButtonX2+1)
    #define MOUSE_BUTTON_TO_SDL_MOUSE_BUTTON(x) ((x)-1)

    /**
    * Mask to get a region of combined mouse button value
    */
    typedef enum {
        MouseButtonMaskLeft = TWO_BIT_MASK(MouseButtonLeft),
        MouseButtonMaskMiddle = TWO_BIT_MASK(MouseButtonMiddle),
        MouseButtonMaskRight = TWO_BIT_MASK(MouseButtonRight),
        MouseButtonMaskX1 = TWO_BIT_MASK(MouseButtonX1),
        MouseButtonMaskX2 = TWO_BIT_MASK(MouseButtonX2),
    } MouseButtonMask;

    /**
    * Converts mouse button index to corresponding mask
    */
    inline MouseButtonMask GetMouseButtonMaskFromMouseButton(const int button)
    {
        return static_cast<MouseButtonMask>(TWO_BIT_MASK(button));
    }

    /**
    * Extracts the trigger value from a combined mouse button value
    */
    inline TriggerState GetTriggerStateFromMouseButtonState(const uint16_t state, const int button)
    {
        return static_cast<TriggerState>((state & TWO_BIT_MASK(button)) >> 2*button);
    }

    /**
    * Converts a trigger value to the corresponding combined value
    */
    inline uint16_t GetMouseButtonStateFromMouseButton(const TriggerState state, const int button)
    {
        return state << button*2;
    }

    /**
    * Wrapper for SDL_KeyCode
    */
    typedef enum
    {
        KeyCodeNone = 0,

        KeyCodeA = SDLK_a,
        KeyCodeB = SDLK_b,
        KeyCodeC = SDLK_c,
        KeyCodeD = SDLK_d,
        KeyCodeE = SDLK_e,
        KeyCodeF = SDLK_f,
        KeyCodeG = SDLK_g,
        KeyCodeH = SDLK_h,
        KeyCodeI = SDLK_i,
        KeyCodeJ = SDLK_j,
        KeyCodeK = SDLK_k,
        KeyCodeL = SDLK_l,
        KeyCodeM = SDLK_m,
        KeyCodeN = SDLK_n,
        KeyCodeO = SDLK_o,
        KeyCodeP = SDLK_p,
        KeyCodeQ = SDLK_q,
        KeyCodeR = SDLK_r,
        KeyCodeS = SDLK_s,
        KeyCodeT = SDLK_t,
        KeyCodeU = SDLK_u,
        KeyCodeV = SDLK_v,
        KeyCodeW = SDLK_w,
        KeyCodeX = SDLK_x,
        KeyCodeY = SDLK_y,
        KeyCodeZ = SDLK_z,

        KeyCode0 = SDLK_0,
        KeyCode1 = SDLK_1,
        KeyCode2 = SDLK_2,
        KeyCode3 = SDLK_3,
        KeyCode4 = SDLK_4,
        KeyCode5 = SDLK_5,
        KeyCode6 = SDLK_6,
        KeyCode7 = SDLK_7,
        KeyCode8 = SDLK_8,
        KeyCode9 = SDLK_9,

        KeyCodeRight = SDLK_RIGHT,
        KeyCodeLeft = SDLK_LEFT,
        KeyCodeDown = SDLK_DOWN,
        KeyCodeUp = SDLK_UP,

        KeyCodeNumpad0 = SDLK_KP_0,
        KeyCodeNumpad1 = SDLK_KP_1,
        KeyCodeNumpad2 = SDLK_KP_2,
        KeyCodeNumpad3 = SDLK_KP_3,
        KeyCodeNumpad4 = SDLK_KP_4,
        KeyCodeNumpad5 = SDLK_KP_5,
        KeyCodeNumpad6 = SDLK_KP_6,
        KeyCodeNumpad7 = SDLK_KP_7,
        KeyCodeNumpad8 = SDLK_KP_8,
        KeyCodeNumpad9 = SDLK_KP_9,
        KeyCodeNumpadDivide = SDLK_KP_DIVIDE,
        KeyCodeNumpadMultiply = SDLK_KP_MULTIPLY,
        KeyCodeNumpadMinus = SDLK_KP_MINUS,
        KeyCodeNumpadPlus = SDLK_KP_PLUS,
        KeyCodeNumpadEnter = SDLK_KP_ENTER,
        KeyCodeNumpadPeriod = SDLK_KP_PERIOD,

        KeyCodeReturn = SDLK_RETURN,
        KeyCodeEscape = SDLK_ESCAPE,
        KeyCodeBackspace = SDLK_BACKSPACE,
        KeyCodeTab = SDLK_TAB,
        KeyCodeSpace = SDLK_SPACE,
        KeyCodeLCtrl = SDLK_LCTRL,
        KeyCodeRCtrl = SDLK_RCTRL,
        KeyCodeLShift = SDLK_LSHIFT,
        KeyCodeRShift = SDLK_RSHIFT,
        KeyCodeLAlt = SDLK_LALT,
        KeyCodeRAlt = SDLK_RALT,

        KeyCodeF1 = SDLK_F1,
        KeyCodeF2 = SDLK_F2,
        KeyCodeF3 = SDLK_F3,
        KeyCodeF4 = SDLK_F4,
        KeyCodeF5 = SDLK_F5,
        KeyCodeF6 = SDLK_F6,
        KeyCodeF7 = SDLK_F7,
        KeyCodeF8 = SDLK_F8,
        KeyCodeF9 = SDLK_F9,
        KeyCodeF10 = SDLK_F10,
        KeyCodeF11 = SDLK_F11,
        KeyCodeF12 = SDLK_F12,

        /*
        SDLK_LGUI = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LGUI),
        SDLK_RGUI = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RGUI),
        SDLK_EXCLAIM = '!',
        SDLK_QUOTEDBL = '"',
        SDLK_HASH = '#',
        SDLK_PERCENT = '%',
        SDLK_DOLLAR = '$',
        SDLK_AMPERSAND = '&',
        SDLK_QUOTE = '\'',
        SDLK_LEFTPAREN = '(',
        SDLK_RIGHTPAREN = ')',
        SDLK_ASTERISK = '*',
        SDLK_PLUS = '+',
        SDLK_COMMA = ',',
        SDLK_MINUS = '-',
        SDLK_PERIOD = '.',
        SDLK_SLASH = '/',
        SDLK_COLON = ':',
        SDLK_SEMICOLON = ';',
        SDLK_LESS = '<',
        SDLK_EQUALS = '=',
        SDLK_GREATER = '>',
        SDLK_QUESTION = '?',
        SDLK_AT = '@',

        SDLK_PRINTSCREEN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN),
        SDLK_SCROLLLOCK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK),
        SDLK_PAUSE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE),
        SDLK_INSERT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT),
        SDLK_HOME = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME),
        SDLK_PAGEUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP),
        SDLK_DELETE = '\x7F',
        SDLK_END = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END),
        SDLK_PAGEDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN),

        SDLK_NUMLOCKCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR),

        SDLK_APPLICATION = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION),
        SDLK_POWER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_POWER),
        SDLK_KP_EQUALS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALS),
        SDLK_F13 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F13),
        SDLK_F14 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F14),
        SDLK_F15 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F15),
        SDLK_F16 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16),
        SDLK_F17 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17),
        SDLK_F18 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18),
        SDLK_F19 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19),
        SDLK_F20 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20),
        SDLK_F21 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21),
        SDLK_F22 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22),
        SDLK_F23 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23),
        SDLK_F24 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24),
        SDLK_EXECUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE),
        SDLK_HELP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HELP),
        SDLK_MENU = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MENU),
        SDLK_SELECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT),
        SDLK_STOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP),
        SDLK_AGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN),
        SDLK_UNDO = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO),
        SDLK_CUT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT),
        SDLK_COPY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY),
        SDLK_PASTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE),
        SDLK_FIND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND),
        SDLK_MUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE),
        SDLK_VOLUMEUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP),
        SDLK_VOLUMEDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN),
        SDLK_KP_COMMA = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COMMA),
        SDLK_KP_EQUALSAS400 =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400),

        SDLK_ALTERASE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE),
        SDLK_SYSREQ = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ),
        SDLK_CANCEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL),
        SDLK_CLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEAR),
        SDLK_PRIOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR),
        SDLK_RETURN2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2),
        SDLK_SEPARATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR),
        SDLK_OUT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT),
        SDLK_OPER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER),
        SDLK_CLEARAGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN),
        SDLK_CRSEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL),
        SDLK_EXSEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL),

        SDLK_KP_00 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_00),
        SDLK_KP_000 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_000),
        SDLK_THOUSANDSSEPARATOR =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR),
        SDLK_DECIMALSEPARATOR =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR),
        SDLK_CURRENCYUNIT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT),
        SDLK_CURRENCYSUBUNIT =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT),
        SDLK_KP_LEFTPAREN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN),
        SDLK_KP_RIGHTPAREN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN),
        SDLK_KP_LEFTBRACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE),
        SDLK_KP_RIGHTBRACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE),
        SDLK_KP_TAB = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_TAB),
        SDLK_KP_BACKSPACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE),
        SDLK_KP_A = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_A),
        SDLK_KP_B = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_B),
        SDLK_KP_C = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_C),
        SDLK_KP_D = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_D),
        SDLK_KP_E = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_E),
        SDLK_KP_F = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_F),
        SDLK_KP_XOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_XOR),
        SDLK_KP_POWER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_POWER),
        SDLK_KP_PERCENT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERCENT),
        SDLK_KP_LESS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LESS),
        SDLK_KP_GREATER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_GREATER),
        SDLK_KP_AMPERSAND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND),
        SDLK_KP_DBLAMPERSAND =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND),
        SDLK_KP_VERTICALBAR =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR),
        SDLK_KP_DBLVERTICALBAR =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR),
        SDLK_KP_COLON = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COLON),
        SDLK_KP_HASH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HASH),
        SDLK_KP_SPACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_SPACE),
        SDLK_KP_AT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AT),
        SDLK_KP_EXCLAM = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM),
        SDLK_KP_MEMSTORE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE),
        SDLK_KP_MEMRECALL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL),
        SDLK_KP_MEMCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR),
        SDLK_KP_MEMADD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMADD),
        SDLK_KP_MEMSUBTRACT =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT),
        SDLK_KP_MEMMULTIPLY =
        SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY),
        SDLK_KP_MEMDIVIDE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE),
        SDLK_KP_PLUSMINUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS),
        SDLK_KP_CLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEAR),
        SDLK_KP_CLEARENTRY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY),
        SDLK_KP_BINARY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BINARY),
        SDLK_KP_OCTAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_OCTAL),
        SDLK_KP_DECIMAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL),
        SDLK_KP_HEXADECIMAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL),

        SDLK_MODE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE),

        SDLK_AUDIONEXT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIONEXT),
        SDLK_AUDIOPREV = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPREV),
        SDLK_AUDIOSTOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOSTOP),
        SDLK_AUDIOPLAY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPLAY),
        SDLK_AUDIOMUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOMUTE),
        SDLK_MEDIASELECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIASELECT),
        SDLK_WWW = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_WWW),
        SDLK_MAIL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MAIL),
        SDLK_CALCULATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALCULATOR),
        SDLK_COMPUTER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COMPUTER),
        SDLK_AC_SEARCH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH),
        SDLK_AC_HOME = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME),
        SDLK_AC_BACK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK),
        SDLK_AC_FORWARD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD),
        SDLK_AC_STOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP),
        SDLK_AC_REFRESH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH),
        SDLK_AC_BOOKMARKS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS),

        SDLK_BRIGHTNESSDOWN =
            SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSDOWN),
        SDLK_BRIGHTNESSUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSUP),
        SDLK_DISPLAYSWITCH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DISPLAYSWITCH),
        SDLK_KBDILLUMTOGGLE =
            SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMTOGGLE),
        SDLK_KBDILLUMDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMDOWN),
        SDLK_KBDILLUMUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMUP),
        SDLK_EJECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EJECT),
        SDLK_SLEEP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP),
        SDLK_APP1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APP1),
        SDLK_APP2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APP2),

        SDLK_AUDIOREWIND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOREWIND),
        SDLK_AUDIOFASTFORWARD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOFASTFORWARD),

        SDLK_SOFTLEFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTLEFT),
        SDLK_SOFTRIGHT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTRIGHT),
        SDLK_CALL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALL),
        SDLK_ENDCALL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ENDCALL)
        */
    } KeyCodes;
}

template<>
struct std::hash<Engine::BindingPair>
{
    std::size_t operator() (const Engine::BindingPair& v) const { return std::hash<int>()(v.DeviceType) ^ hash<int>()(v.BindingId) << 1; }
};
