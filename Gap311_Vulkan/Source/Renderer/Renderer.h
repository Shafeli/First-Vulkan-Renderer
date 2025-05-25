#pragma once
#include "../CoreSystems/CoreSystems.h"

struct SDL_Renderer;
struct SDL_Texture;

namespace Brokkr
{
    class WindowHandle;
    class Color;

    class Renderer final : public CoreSubsystem
    {
        SDL_Renderer* m_pRenderer = nullptr;

    public:
        explicit Renderer(CoreSystems* pCoreManager)
            : CoreSubsystem(pCoreManager)
        {
            //
        }

        virtual ~Renderer() override;

        void Init(const WindowHandle* pWindow);
        void ClearRenderer();

        void RenderCopy(SDL_Texture* texture, int x, int y, int w, int h);
        void Render() const;

        void SetRenderDrawColor(int red, int green, int blue, int opacity) const;

        void RenderCircle(int centerX, int centerY, int radius, int red, int green, int blue) const;
        void RenderSquare(int x, int y, int h, int w, int red, int green, int blue, int opacity) const;
        void RenderLine(int x1, int y1, int x2, int y2, int red, int green, int blue) const;

        [[nodiscard]] SDL_Renderer* GetRenderer() const { return m_pRenderer; }

        virtual void Destroy() override;
    };
}
