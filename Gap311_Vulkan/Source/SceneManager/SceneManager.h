#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "BrokkrDefaultScene.h"
#include "Scene.h"
#include "../CoreSystems/CoreSystems.h"


namespace Brokkr
{
    // TODO: redo adding a state to fit the engine general design for type convertion adding templates
    class SceneManager final : public CoreSubsystem
    {
        std::unordered_map<std::string, std::unique_ptr<Scene>> m_states;
        std::string m_pActiveStateKey;
        std::string m_pendingStateKey;

        Scene* m_pActiveState = nullptr;
        bool m_isReplacing = false;

    public:
        explicit SceneManager(CoreSystems* pCoreManager)
            : CoreSubsystem(pCoreManager)
        {
            //
            AddState("BrokkrEngineDefault", std::make_unique<BrokkrDefaultScene>(pCoreManager));
            SetActiveState("BrokkrEngineDefault");
        }

        virtual ~SceneManager() override;

        //TODO: move these voids to bool and return is was successful or not 
        void AddState(const std::string& stateIdentifier, std::unique_ptr<Scene> newState);
        void RemoveState(const std::string& stateIdentifier);
        void SetActiveState(const std::string& stateIdentifier);

        virtual void Destroy() override;

    protected: // Only calls these two in core main game loop
        friend class CoreSystems;
        void ProcessStateChange();
        void UpdateActive();
        [[nodiscard]] Scene* GetActiveState();
        
    };
}

