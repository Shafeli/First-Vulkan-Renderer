#pragma once

#include "../CoreSystems/CoreSystems.h"

struct SDL_Window;

namespace Brokkr
{
    class Color;
    class CoreSubsystem;
    class CoreSystems;

    class WindowHandle final : public CoreSubsystem
    {
        SDL_Window* m_pWindow = nullptr;
        std::string m_screenTitle;
        int m_screenWidth;
        int m_screenHeight;

        int m_red;
        int m_green;
        int m_blue;
        int m_opacity;

    public:
        explicit WindowHandle(CoreSystems* pCoreManager)
            : CoreSubsystem(pCoreManager)
            , m_screenWidth(0)
            , m_screenHeight(0)
            , m_red(0)
            , m_green(0)
            , m_blue(0)
            , m_opacity(0)
        {
            //
        }

        void SetBackgroundColor(int red, int green, int blue, int opacity);
        bool InitWindow(const char* screenTitle, int screenWidth, int screenHeight);

        [[nodiscard]] SDL_Window* GetWindow() const { return m_pWindow; }
        [[nodiscard]] std::string& GetWindowName() { return m_screenTitle; }
        [[nodiscard]] int GetScreenWidth() const { return m_screenWidth; }
        [[nodiscard]] int GetScreenHeight() const { return m_screenHeight; }

        virtual void Destroy() override;
    };

}

