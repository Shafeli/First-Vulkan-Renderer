#pragma once
#include "../../CoreSystems/CoreSubsystem/CoreSubsystem.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <filesystem>
#include "../../gap-311/VulkanShaderLibrary.hpp"

// Forward declarations
struct GraphicsPipeline;
struct GraphicsPipelineConfig;
struct ShaderUniform;
class VulkanShaderLibrary;

class VulkanPipelineManager final : public Brokkr::CoreSubsystem
{
public:
    explicit VulkanPipelineManager(Brokkr::CoreSystems* pCoreManager)
        : CoreSubsystem(pCoreManager)
    {

    }

    virtual void Destroy() override;
};