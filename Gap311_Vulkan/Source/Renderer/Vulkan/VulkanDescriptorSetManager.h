#pragma once
#include "../../CoreSystems/CoreSubsystem/CoreSubsystem.h"

class VulkanDescriptorSetManager final : public Brokkr::CoreSubsystem
{

public:
    explicit VulkanDescriptorSetManager(Brokkr::CoreSystems* pCoreManager)
        : CoreSubsystem(pCoreManager)
    {
        //
    }

    virtual void Destroy() override;
};

