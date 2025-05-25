#pragma once

//TODO: add an init to interface in next refactor
namespace Brokkr
{
    class CoreSystems;

    class CoreSubsystem
    {

    public:
        explicit CoreSubsystem(CoreSystems* pCoreManager)
            : m_pCoreManager(pCoreManager)
        {
            //
        }

        virtual ~CoreSubsystem() = default;
        virtual void Destroy() = 0;

    protected:
        CoreSystems* m_pCoreManager;
    };

}
