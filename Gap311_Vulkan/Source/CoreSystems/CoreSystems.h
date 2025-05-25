#pragma once
#include <chrono>
#include <memory>
#include <vector>
#include <SDL2/SDL_events.h>

#include "CoreSubsystem/CoreSubsystem.h"
#include "DeltaTime.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Core Systems:
// TODO: Core Systems Tagged for Re-design
// TODO: Docs and how to use
// TODO: Add init() and removal sorting for shut downs & start ups
/////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Brokkr
{
    class SceneManager;
    class InputManager;

    class CoreSystems
    {
   
        std::vector<std::unique_ptr<CoreSubsystem>> m_pCoreSubsystems;
        Brokkr::DeltaTime m_deltaTime;
        Brokkr::InputManager* m_pInputManager = nullptr;
        Brokkr::SceneManager* m_pSceneManager = nullptr;


    public:

        SDL_Event* m_pFrameEvent = nullptr;
        Brokkr::DeltaTime* GetDeltaTime() { return &m_deltaTime; }

        bool Init(const char* pGameTitle, int screenWidth, int screenHeight); // For when system is building

        void Run();

        template <typename CoreSubsystem>
        CoreSubsystem* GetCoreSystem()
        {
            // Iterate through all components in the vector
            for (auto& subsystem : m_pCoreSubsystems)
            {
                // If the cast is successful
                if (CoreSubsystem* target = dynamic_cast<CoreSubsystem*>(subsystem.get()))
                {
                    //target->AddRef(); // TODO: remove ref counting in core systems
                    return target;
                }
            }

            return nullptr; // If no system of type is found
        }

        template <typename CoreSubsystem, typename ... Args>
        CoreSubsystem* AddCoreSystem(Args&&... args)
        {
            std::unique_ptr<CoreSubsystem> newCoreSubsystem = std::make_unique<CoreSubsystem>(this, std::forward<Args>(args)...);

            CoreSubsystem* result = newCoreSubsystem.get(); // Get a raw pointer to the component
            m_pCoreSubsystems.emplace_back(std::move(newCoreSubsystem)); // Add the system to the vector

            if (!result)
            {

            }
            return result; // Return a pointer
        }

        template<typename CoreSubsystem>
        void RemoveCoreSystem(CoreSubsystem* System)
        {
            // Iterate through all components in the vector
            for (size_t i = 0; i < m_pCoreSubsystems.size(); ++i)
            {
                // If the cast is successful
                if (CoreSubsystem* target = dynamic_cast<CoreSubsystem*>(m_pCoreSubsystems[i].get()))
                {
                    if (target == System) // If the found type is the pointer we already have then
                    {

                        const int remainingRefs = target->Release(); // relase returns a number == the remaining Refs to  this system

                        if (remainingRefs <= 0) // If no more ref remove the ptr
                        {
                            target->Destroy();
                            // Swap and pop the element at index i
                            std::swap(m_pCoreSubsystems[i], m_pCoreSubsystems.back());
                            m_pCoreSubsystems.pop_back();
                            break; // Stop searching
                        }
                    }
                }
            }
        }

        void Destroy() const;
    };
}
