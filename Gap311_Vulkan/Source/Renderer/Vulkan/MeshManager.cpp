#include "MeshManager.h"

std::shared_ptr<Mesh<Brokkr::MeshVertex>> MeshManager::Get(const char* pMeshPath)
{
    auto it = m_meshData.find(std::string(pMeshPath));
    if (it != m_meshData.end())
    {
        return it->second;
    }

    return nullptr;
}

void MeshManager::Add(const char* pMeshPath)
{
    auto it = m_meshData.find(std::string(pMeshPath));
    if (it != m_meshData.end())
    {
        return; // defaulting to not overriding 
    }

    const auto result = m_meshData.emplace(pMeshPath, std::make_shared<Mesh<Brokkr::MeshVertex>>(m_VulkanContext));
    //result.first->second->LoadFromFile(pMeshPath);
}

void MeshManager::Remove(const char* PMeshPath)
{
    auto it = m_meshData.find(std::string(PMeshPath));
    if (it != m_meshData.end())
    {
        m_meshData.erase(it);
    }

}

void MeshManager::DumpAllMesh()
{
    m_meshData.clear();
}

void MeshManager::Destroy()
{
    //
    DumpAllMesh();
}
