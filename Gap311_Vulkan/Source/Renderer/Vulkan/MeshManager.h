#pragma once
#include "Mesh.h"
#include "../../CoreSystems/CoreSystems.h"

class MeshManager final : public Brokkr::CoreSubsystem
{
    std::unordered_map<std::string, std::shared_ptr<Mesh<Brokkr::MeshVertex>>> m_meshData;
    GAP311::VulkanWrapper& m_VulkanContext;

public:

    MeshManager(Brokkr::CoreSystems* pCoreManager, GAP311::VulkanWrapper& VulkanContext)
        : CoreSubsystem(pCoreManager)
        , m_VulkanContext(VulkanContext)
    {
        //
    }

    std::shared_ptr<Mesh<Brokkr::MeshVertex>> Get(const char* pMeshPath);
    void Add(const char* pMeshPath);
    void Remove(const char* PMeshPath);
    void DumpAllMesh();

    virtual void Destroy() override;
};

