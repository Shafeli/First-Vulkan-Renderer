#include "WindowHandle.h"
#include <SDL_image.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>


void Brokkr::WindowHandle::SetBackgroundColor(int red, int green, int blue, int opacity)
{
    m_red = red;
    m_green = green;
    m_blue = blue;
    m_opacity = opacity;
}

bool Brokkr::WindowHandle::InitWindow(const char* screenTitle, int screenWidth, int screenHeight)
{
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    SDL_Rect windowRect = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight };
    m_pWindow = SDL_CreateWindow(screenTitle, windowRect.x, windowRect.y, windowRect.w, windowRect.h, SDL_WINDOW_VULKAN);
    if (!m_pWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    return true;
}

void Brokkr::WindowHandle::Destroy()
{
    SDL_DestroyWindow(m_pWindow);

    // Quit SDL_image
    IMG_Quit();
    SDL_Quit();
}
