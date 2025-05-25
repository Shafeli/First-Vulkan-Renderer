#include "CoreSystems.h"

#include <SDL2/SDL_events.h>

#include "../InputManager/InputManager.h"
#include "../SceneManager/SceneManager.h"
#include "../WindowHandle/WindowHandle.h"
#include "../XMLManager/XMLManager.h"

#define  FRAME_TRACKER 0


class IMGUIManager;

bool Brokkr::CoreSystems::Init(const char* pGameTitle, int screenWidth, int screenHeight)
{

    const auto pWindowHandle = AddCoreSystem<WindowHandle>();
    m_pInputManager = AddCoreSystem<InputManager>();
    m_pSceneManager = AddCoreSystem<SceneManager>();
    AddCoreSystem<XMLManager>();

    return true;
}

void Brokkr::CoreSystems::Run()
{

#if  FRAME_TRACKER
    static int frameCount = 0;
    static double frameTime = 0;
#endif

    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    SDL_Event windowEvent;

    while (m_pInputManager->IsKeyPressed(InputManager::KeyCode::ESC) == InputManager::kNoInputCode)
    {

#if  FRAME_TRACKER
        ++frameCount;
#endif

        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                break;
            }
        }
        m_pFrameEvent = &windowEvent;

        m_pSceneManager->ProcessStateChange();

        std::chrono::time_point<std::chrono::steady_clock> thisFrameTime;
        thisFrameTime = std::chrono::high_resolution_clock::now();

        //Time between frames 
        std::chrono::duration<double> lastFrameDuration = thisFrameTime - lastFrameTime;

        //Sets the Master copy of Delta Time
        m_deltaTime.SetTime(lastFrameDuration.count());

        lastFrameTime = thisFrameTime;

        m_pInputManager->Update();
        m_pSceneManager->UpdateActive();

#if  FRAME_TRACKER
        frameTime += lastFrameDuration.count() * 1000.0;
        if (frameTime >= 1000)
        {
            std::string testing = "FPS: ";
            testing += std::to_string(frameCount);
            m_logger.Log(Logger::LogLevel::kDebug, testing);

            frameCount = 0;
            frameTime -= 1000.0;
        }
#endif
    }
}

void Brokkr::CoreSystems::Destroy() const
{

    for (const auto & pCoreSubsystem : m_pCoreSubsystems)
    {
        pCoreSubsystem->Destroy();
    }
}





