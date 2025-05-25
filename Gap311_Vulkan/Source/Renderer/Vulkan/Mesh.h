#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "../../gap-311/VulkanWrapper.hpp"
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include "../Vulkan/VulkanRenderer.h"



namespace Brokkr
{
    struct MeshVertex;
    struct DebugVertex;
}

template<typename VertexType>
class Mesh
{
    GAP311::VulkanWrapper& m_VulkanContext;

public:

    // mat details 
    const float m_uvScale = 1.0f; // 0 Scaling UV
    glm::vec4 color;

    uint32_t vertexCount;
    uint32_t indexCount;
    vk::Buffer vertexBuffer;
    vk::Buffer indexBuffer;

    std::vector<VertexType> vertices;       // Positions of the vertices
    std::vector<uint32_t> indices;                  // Index buffer for element order

    Mesh(GAP311::VulkanWrapper& VulkanContext, float uvScale)
        : m_VulkanContext(VulkanContext)
        , m_uvScale(uvScale)
        , color()
        , vertexCount(0)
        , indexCount(0)

    {
        //
    }

    Mesh(GAP311::VulkanWrapper& VulkanContext)
        : m_VulkanContext(VulkanContext)
        , color()
        , vertexCount(0)
        , indexCount(0)
    {
        //
    }

    void Destroy()
    {
        if (vertexBuffer) m_VulkanContext.DestroyBuffer(vertexBuffer);
        if (indexBuffer)  m_VulkanContext.DestroyBuffer(indexBuffer);
    }

    void Load()
    {
        Destroy();

        vertexCount = static_cast<uint32_t>(vertices.size());
        indexCount = static_cast<uint32_t>(indices.size());

        vertexBuffer = m_VulkanContext.CreateVertexBuffer(vertices.size() * sizeof(VertexType), vertices.data());
        indexBuffer = m_VulkanContext.CreateIndexBuffer(indices.size() * sizeof(uint32_t), indices.data());
    }
};

