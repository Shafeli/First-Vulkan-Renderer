#include "InputManager.h"
#include <SDL.h>

void Brokkr::InputManager::Update()
{
    for (size_t i = 0; i < kNumInput; ++i)
    {
        m_activeInput[i] = InputCheck(static_cast<KeyCode>(kKeyCodes[i]));
    }

    // Update mouse state
    int x, y;
    Uint32 mouseState = SDL_GetMouseState(&x, &y);
    m_mouseX = x;
    m_mouseY = y;
}

float Brokkr::InputManager::IsKeyPressed(KeyCode key)
{
    size_t index = 0;
    for (size_t i = 0; i < kNumInput; ++i)
    {
        if (static_cast<KeyCode>(kKeyCodes[i]) == key)
        {
            index = i;
            break;
        }
    }

    if (m_activeInput[index])
    {
        return m_holdTimes[index].GetElapsedTimeInSeconds();
    }
    m_holdTimes[index].Restart();
    return kNoInputCode;
}

void Brokkr::InputManager::UpdateKeyCode(KeyCode key)
{
    const auto index = static_cast<size_t>(key);
    m_activeInput[index] = InputCheck(key);
}

bool Brokkr::InputManager::InputCheck(KeyCode key)
{
    const auto index = static_cast<size_t>(key);
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    return state[index];
}

void Brokkr::InputManager::Destroy()
{
    //
}

Brokkr::InputManager::KeyCode Brokkr::InputManager::ToKeyCode(const char* key)
{
    const std::string comparisionStr = key;

    if (const auto iterator = keyCodeMap.find(comparisionStr); iterator != keyCodeMap.end()) 
    {
        return iterator->second;
    }

    return static_cast<KeyCode>(-1);
}

float Brokkr::InputManager::GetMouseX()
{
    return static_cast<float>(m_mouseX);
}

float Brokkr::InputManager::GetMouseY()
{
    return static_cast<float>(m_mouseY);
}

