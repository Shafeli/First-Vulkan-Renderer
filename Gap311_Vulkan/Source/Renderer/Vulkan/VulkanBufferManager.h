#pragma once
#include "../../CoreSystems/CoreSubsystem/CoreSubsystem.h"
#include "../../gap-311/VulkanWrapper.hpp"


struct VulkanBuffer
{
    vk::Buffer buffer;
    vk::DeviceMemory memory;
};

class VulkanBufferManager : public Brokkr::CoreSubsystem
{
    GAP311::VulkanWrapper& m_VulkanContext;
public:
    explicit VulkanBufferManager(Brokkr::CoreSystems* pCoreManager, GAP311::VulkanWrapper& VulkanContext)
        : CoreSubsystem(pCoreManager)
        , m_VulkanContext(VulkanContext)
    {
        //
    }

    // Idea is to have the buffers for index and vertex share the same key so two maps one for vertex and one for index

    virtual void Destroy() override;
};

