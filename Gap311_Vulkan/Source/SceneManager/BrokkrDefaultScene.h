#pragma once
#include "Scene.h"

namespace Brokkr
{
    class BrokkrDefaultScene final : public Scene
    {

    public:
        explicit BrokkrDefaultScene(CoreSystems* pEngineSystems)
            : Scene(pEngineSystems)
        {
            //
        }

        virtual void Init() override;
        virtual void Update() override;
        virtual void Exit() override;
        virtual void Enter() override;
        virtual void Destroy() override;
    };

}

