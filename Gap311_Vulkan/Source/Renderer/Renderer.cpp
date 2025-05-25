#include "Renderer.h"
#include <SDL2/SDL_render.h>
#include "../WindowHandle/WindowHandle.h"

Brokkr::Renderer::~Renderer()
{
    Destroy();
}

void Brokkr::Renderer::Init(const WindowHandle* pWindow)
{
    m_pRenderer = SDL_CreateRenderer(pWindow->GetWindow(), -1, SDL_RENDERER_ACCELERATED);
}

void Brokkr::Renderer::ClearRenderer()
{
    SDL_RenderClear(m_pRenderer);
}

void Brokkr::Renderer::RenderCopy(SDL_Texture* texture, int x, int y, int w, int h)
{
    SDL_Rect destination;
    destination.x = x;
    destination.y = y;
    destination.w = w;
    destination.h = h;

    SDL_RenderCopy(m_pRenderer, texture, nullptr, &destination);
}

void Brokkr::Renderer::Render() const
{
    SDL_RenderPresent(m_pRenderer);
}

void Brokkr::Renderer::SetRenderDrawColor(int red, int green, int blue, int opacity) const
{
    SDL_SetRenderDrawColor
    (
        m_pRenderer,
        static_cast<Uint8>(red),
        static_cast<Uint8>(green),
        static_cast<Uint8>(blue),
        static_cast<Uint8>(opacity)
    );
}

void Brokkr::Renderer::RenderCircle(int centerX, int centerY, int radius, int red, int green, int blue) const
{
    SDL_SetRenderDrawColor
    (
        m_pRenderer,
        static_cast<Uint8>(red),
        static_cast<Uint8>(green),
        static_cast<Uint8>(blue),
        SDL_ALPHA_OPAQUE
    );

    // the midpoint circle algorithm
    // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm has the explanation
    int x = radius;
    int y = 0;
    int distance = 0;

    while (x >= y)
    {
        SDL_RenderDrawPoint(m_pRenderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(m_pRenderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(m_pRenderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(m_pRenderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(m_pRenderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(m_pRenderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(m_pRenderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(m_pRenderer, centerX + x, centerY - y);

        if (distance <= 0)
        {
            y += 1;
            distance += 2 * y + 1;
        }

        if (distance > 0)
        {
            x -= 1;
            distance -= 2 * x + 1;
        }
    }

}

void Brokkr::Renderer::RenderSquare(int x, int y, int h, int w, int red, int green, int blue, int opacity) const
{
    SDL_SetRenderDrawColor
    (
        m_pRenderer,
        static_cast<Uint8>(red),
        static_cast<Uint8>(green),
        static_cast<Uint8>(blue),
        static_cast<Uint8>(opacity)
    );

    SDL_Rect rect;
    rect.h = h;
    rect.w = w;
    rect.x = x;
    rect.y = y;

    SDL_RenderFillRect(m_pRenderer, &rect);
}

void Brokkr::Renderer::RenderLine(int x1, int y1, int x2, int y2, int red, int green, int blue) const
{
    SDL_SetRenderDrawColor
    (
        m_pRenderer,
        static_cast<Uint8>(red),
        static_cast<Uint8>(green),
        static_cast<Uint8>(blue),
        SDL_ALPHA_OPAQUE
    );

    SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
}

void Brokkr::Renderer::Destroy()
{
    if (m_pRenderer != nullptr)
        SDL_DestroyRenderer(m_pRenderer);
}
