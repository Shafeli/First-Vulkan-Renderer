#pragma once
#include <unordered_map>

#include "../CoreSystems/CoreSystems.h"
#include "../Utilities/Clock.h"


/////////////////////////////////////////////////
//
//      Source: 
//      this repo https://gist.github.com/stuartpb/804078/18faabdc0310d864bd38a39ae8d9bdf7794572cd
//      combined documentation for SDL_ScanCode and SDLKey
//
//           InputManager
//
// Requesting if a key is pressed returns a float of how long in seconds 
// the key has been pressed or returns kNoInputCode to indicate no input.
//
// Adding a key Steps:
//
// 1. Add the key to the enum class:
//    - Append a new entry to the enum class `KeyCode` and assign it the corresponding SDL scan code.
//      For example, if the SDL scan code for the key is 50, you can add:
//      MyKey = 50, but KEEPING things in order helps!
//
// 2. Update the constant `kKeyCodes` array:
//    - Add the scan code of the new key to the `kKeyCodes` array in the same order as the enum class entries.
//      
//
// 3. Update the `keyCodeMap` unordered map:
//    - Add a key-value pair to the `keyCodeMap` map for convenient conversion between key strings and key codes.
//      The key should be a string representing the key name, and the value should be the corresponding `KeyCode`.
//      For example, if the key is "FooKey", you can add:
//      {"FooKey", KeyCode::FooKey}
//
// 4. Update the `kNumInput` constant:
//    - Increment the `kNumInput` constant by 1 for each key added to the system. This constant represents the
//      total number of keys available to the system.
//
// How to InputManager:
//  TODO:
//
//
/////////////////////////////////////////////////


namespace Brokkr
{
    class InputManager final : public CoreSubsystem
    {
    public:
        static constexpr float kNoInputCode = -1.0;

        // Enums match SDL_SCANCODES for now https://wiki.libsdl.org/SDL2/SDL_Scancode
        enum class KeyCode
        {
            A = 4,
            B = 5,
            C = 6,
            D = 7,
            E = 8,
            F = 9,
            G = 10,
            H = 11,
            I = 12,
            J = 13,
            K = 14,
            L = 15,
            M = 16,
            N = 17,
            O = 18,
            P = 19,
            Q = 20,
            R = 21,
            S = 22,
            T = 23,
            U = 24,
            V = 25,
            W = 26,
            X = 27,
            Y = 28,
            Z = 29,
            Num1 = 30,
            Num2 = 31,
            Num3 = 32,
            Num4 = 33,
            Num5 = 34,
            Num6 = 35,
            Num7 = 36,
            Num8 = 37,
            Num9 = 38,
            Num0 = 39,
            Return = 40,
            ESC = 41,
            Backspace = 42,
            Tab = 43,
            Space = 44,
            Minus = 45,
            Equals = 46,
            LeftBracket = 47,
            RightBracket = 48,
            Backslash = 49,
            Semicolon = 51,
            Quote = 52,
            Grave = 53,
            Comma = 54,
            Period = 55,
            Slash = 56,
            CapsLock = 57,
            F1 = 58,
            F2 = 59,
            F3 = 60,
            F4 = 61,
            F5 = 62,
            F6 = 63,
            F7 = 64,
            F8 = 65,
            F9 = 66,
            F10 = 67,
            F11 = 68,
            F12 = 69,
            PrintScreen = 70,
            ScrollLock = 71,
            Pause = 72,
            Insert = 73,
            Home = 74,
            PageUp = 75,
            Delete = 76,
            End = 77,
            PageDown = 78,
            Right = 79,
            Left = 80,
            Down = 81,
            Up = 82,
            NumLockClear = 83,
            NumPadDivide = 84,
            NumPadMultiply = 85,
            NumPadMinus = 86,
            NumPadPlus = 87,
            NumPadEnter = 88,
            NumPad1 = 89,
            NumPad2 = 90,
            NumPad3 = 91,
            NumPad4 = 92,
            NumPad5 = 93,
            NumPad6 = 94,
            NumPad7 = 95,
            NumPad8 = 96,
            NumPad9 = 97,
            NumPad0 = 98,
            NumPadPeriod = 99,
        };

    private:

        static constexpr int kKeyCodes[] =
        {
            static_cast<int>(KeyCode::W),
            static_cast<int>(KeyCode::A),
            static_cast<int>(KeyCode::S),
            static_cast<int>(KeyCode::D),
            static_cast<int>(KeyCode::ESC),
            static_cast<int>(KeyCode::Space),
            static_cast<int>(KeyCode::E),
            static_cast<int>(KeyCode::F),
            static_cast<int>(KeyCode::G),
            static_cast<int>(KeyCode::H),
            static_cast<int>(KeyCode::I),
            static_cast<int>(KeyCode::J),
            static_cast<int>(KeyCode::K),
            static_cast<int>(KeyCode::L),
            static_cast<int>(KeyCode::M),
            static_cast<int>(KeyCode::N),
            static_cast<int>(KeyCode::O),
            static_cast<int>(KeyCode::P),
            static_cast<int>(KeyCode::Q),
            static_cast<int>(KeyCode::R),
            static_cast<int>(KeyCode::T),
            static_cast<int>(KeyCode::U),
            static_cast<int>(KeyCode::V),
            static_cast<int>(KeyCode::W),
            static_cast<int>(KeyCode::X),
            static_cast<int>(KeyCode::Y),
            static_cast<int>(KeyCode::Z),
            static_cast<int>(KeyCode::Num1),
            static_cast<int>(KeyCode::Num2),
            static_cast<int>(KeyCode::Num3),
            static_cast<int>(KeyCode::Num4),
            static_cast<int>(KeyCode::Num5),
            static_cast<int>(KeyCode::Num6),
            static_cast<int>(KeyCode::Num7),
            static_cast<int>(KeyCode::Num8),
            static_cast<int>(KeyCode::Num9),
            static_cast<int>(KeyCode::Num0),
            static_cast<int>(KeyCode::Return),
            static_cast<int>(KeyCode::Backspace),
            static_cast<int>(KeyCode::Tab),
            static_cast<int>(KeyCode::Minus),
            static_cast<int>(KeyCode::Equals),
            static_cast<int>(KeyCode::LeftBracket),
            static_cast<int>(KeyCode::RightBracket),
            static_cast<int>(KeyCode::Backslash),
            static_cast<int>(KeyCode::Semicolon),
            static_cast<int>(KeyCode::Quote),
            static_cast<int>(KeyCode::Grave),
            static_cast<int>(KeyCode::Comma),
            static_cast<int>(KeyCode::Period),
            static_cast<int>(KeyCode::Slash),
            static_cast<int>(KeyCode::CapsLock),
            static_cast<int>(KeyCode::F1),
            static_cast<int>(KeyCode::F2),
            static_cast<int>(KeyCode::F3),
            static_cast<int>(KeyCode::F4),
            static_cast<int>(KeyCode::F5),
            static_cast<int>(KeyCode::F6),
            static_cast<int>(KeyCode::F7),
            static_cast<int>(KeyCode::F8),
            static_cast<int>(KeyCode::F9),
            static_cast<int>(KeyCode::F10),
            static_cast<int>(KeyCode::F11),
            static_cast<int>(KeyCode::F12),
            static_cast<int>(KeyCode::PrintScreen),
            static_cast<int>(KeyCode::ScrollLock),
            static_cast<int>(KeyCode::Pause),
            static_cast<int>(KeyCode::Insert),
            static_cast<int>(KeyCode::Home),
            static_cast<int>(KeyCode::PageUp),
            static_cast<int>(KeyCode::Delete),
            static_cast<int>(KeyCode::End),
            static_cast<int>(KeyCode::PageDown),
            static_cast<int>(KeyCode::Right),
            static_cast<int>(KeyCode::Left),
            static_cast<int>(KeyCode::Down),
            static_cast<int>(KeyCode::Up),
            static_cast<int>(KeyCode::NumLockClear),
            static_cast<int>(KeyCode::NumPadDivide),
            static_cast<int>(KeyCode::NumPadMultiply),
            static_cast<int>(KeyCode::NumPadMinus),
            static_cast<int>(KeyCode::NumPadPlus),
            static_cast<int>(KeyCode::NumPadEnter),
            static_cast<int>(KeyCode::NumPad1),
            static_cast<int>(KeyCode::NumPad2),
            static_cast<int>(KeyCode::NumPad3),
            static_cast<int>(KeyCode::NumPad4),
            static_cast<int>(KeyCode::NumPad5),
            static_cast<int>(KeyCode::NumPad6),
            static_cast<int>(KeyCode::NumPad7),
            static_cast<int>(KeyCode::NumPad8),
            static_cast<int>(KeyCode::NumPad9),
            static_cast<int>(KeyCode::NumPad0),
            static_cast<int>(KeyCode::NumPadPeriod)
        };

        // Define map for conversations
        inline static std::unordered_map<std::string, KeyCode> keyCodeMap =
        {
            {"W", KeyCode::W},
            {"A", KeyCode::A},
            {"S", KeyCode::S},
            {"D", KeyCode::D},
            {"ESC", KeyCode::ESC},
            {"Space", KeyCode::Space},
            {"E", KeyCode::E},
            {"F", KeyCode::F},
            {"G", KeyCode::G},
            {"H", KeyCode::H},
            {"I", KeyCode::I},
            {"J", KeyCode::J},
            {"K", KeyCode::K},
            {"L", KeyCode::L},
            {"M", KeyCode::M},
            {"N", KeyCode::N},
            {"O", KeyCode::O},
            {"P", KeyCode::P},
            {"Q", KeyCode::Q},
            {"R", KeyCode::R},
            {"T", KeyCode::T},
            {"U", KeyCode::U},
            {"V", KeyCode::V},
            {"W", KeyCode::W},
            {"X", KeyCode::X},
            {"Y", KeyCode::Y},
            {"Z", KeyCode::Z},
            {"Num1", KeyCode::Num1},
            {"Num2", KeyCode::Num2},
            {"Num3", KeyCode::Num3},
            {"Num4", KeyCode::Num4},
            {"Num5", KeyCode::Num5},
            {"Num6", KeyCode::Num6},
            {"Num7", KeyCode::Num7},
            {"Num8", KeyCode::Num8},
            {"Num9", KeyCode::Num9},
            {"Num0", KeyCode::Num0},
            {"Return", KeyCode::Return},
            {"Backspace", KeyCode::Backspace},
            {"Tab", KeyCode::Tab},
            {"Minus", KeyCode::Minus},
            {"Equals", KeyCode::Equals},
            {"LeftBracket", KeyCode::LeftBracket},
            {"RightBracket", KeyCode::RightBracket},
            {"Backslash", KeyCode::Backslash},
            {"Semicolon", KeyCode::Semicolon},
            {"Quote", KeyCode::Quote},
            {"Grave", KeyCode::Grave},
            {"Comma", KeyCode::Comma},
            {"Period", KeyCode::Period},
            {"Slash", KeyCode::Slash},
            {"CapsLock", KeyCode::CapsLock},
            {"F1", KeyCode::F1},
            {"F2", KeyCode::F2},
            {"F3", KeyCode::F3},
            {"F4", KeyCode::F4},
            {"F5", KeyCode::F5},
            {"F6", KeyCode::F6},
            {"F7", KeyCode::F7},
            {"F8", KeyCode::F8},
            {"F9", KeyCode::F9},
            {"F10", KeyCode::F10},
            {"F11", KeyCode::F11},
            {"F12", KeyCode::F12},
            {"PrintScreen", KeyCode::PrintScreen},
            {"ScrollLock", KeyCode::ScrollLock},
            {"Pause", KeyCode::Pause},
            {"Insert", KeyCode::Insert},
            {"Home", KeyCode::Home},
            {"PageUp", KeyCode::PageUp},
            {"Delete", KeyCode::Delete},
            {"End", KeyCode::End},
            {"PageDown", KeyCode::PageDown},
            {"Right", KeyCode::Right},
            {"Left", KeyCode::Left},
            {"Down", KeyCode::Down},
            {"Up", KeyCode::Up},
            {"NumLockClear", KeyCode::NumLockClear},
            {"NumPadDivide", KeyCode::NumPadDivide},
            {"NumPadMultiply", KeyCode::NumPadMultiply},
            {"NumPadMinus", KeyCode::NumPadMinus},
            {"NumPadPlus", KeyCode::NumPadPlus},
            {"NumPadEnter", KeyCode::NumPadEnter},
            {"NumPad1", KeyCode::NumPad1},
            {"NumPad2", KeyCode::NumPad2},
            {"NumPad3", KeyCode::NumPad3},
            {"NumPad4", KeyCode::NumPad4},
            {"NumPad5", KeyCode::NumPad5},
            {"NumPad6", KeyCode::NumPad6},
            {"NumPad7", KeyCode::NumPad7},
            {"NumPad8", KeyCode::NumPad8},
            {"NumPad9", KeyCode::NumPad9},
            {"NumPad0", KeyCode::NumPad0},
            {"NumPadPeriod", KeyCode::NumPadPeriod}
        };

        static constexpr int kNumInput = 94;

        bool m_activeInput[kNumInput] = { };
        Clock m_holdTimes[kNumInput] = { };
        int m_mouseX;
        int m_mouseY;

    protected:

        friend class CoreSystems;

    public:
        explicit InputManager(CoreSystems* pCoreManager)
            : CoreSubsystem(pCoreManager)
        {
            //
        }

    protected:
        // O(n) for every key 
        void Update();

    public:

        // Checks for a key being pressed down and returns the amount of time it has been held 
        [[nodiscard]] float IsKeyPressed(KeyCode key);
        virtual void Destroy() override;

        // Caller of ToKeyCode should check for the invalid value (static_cast<KeyCode>(-1))
        // to handle a case where the conversion from the input string to the KeyCode enum was unsuccessful.
        KeyCode ToKeyCode(const char* key);
        float GetMouseX();
        float GetMouseY();
    private:
        void UpdateKeyCode(KeyCode key);
        static bool InputCheck(KeyCode key);
    };
}

