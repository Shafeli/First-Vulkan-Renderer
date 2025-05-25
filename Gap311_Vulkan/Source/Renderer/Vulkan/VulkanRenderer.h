#pragma once

#include <queue>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../../gap-311/VulkanShaderLibrary.hpp"
#include "../../gap-311/VulkanWrapper.hpp"
#include "../../CoreSystems/CoreSystems.h"
#include "../../CameraSystem/Camera.h"
#include "Texture.h"

//TODO: Renderer Redesign & Implementation

namespace Brokkr
{
    struct MeshVertex;
}

template<typename T>
class Mesh;

class IMGUIManager;
class VulkanDescriptorSetManager;

class VulkanPipelineManager;

class RenderObject
{
public:
    glm::mat4 transform;
    std::shared_ptr<Mesh<Brokkr::MeshVertex>> mesh;
    std::shared_ptr <Texture> texture;

    RenderObject(const glm::mat4& transformMatrix, std::shared_ptr<Mesh<Brokkr::MeshVertex>> meshRef, std::shared_ptr<Texture> textureRef)
        : transform(transformMatrix)
        , mesh(meshRef)
        , texture(textureRef)
    {
        //
    }
};

namespace Brokkr
{
    // I tried to make a debug box shader and failed but i want to continue working on it
    // and see it through 
    struct DebugBoxPushConstants
    {
        glm::mat4 objectMatrix = glm::identity<glm::mat4>();
        glm::vec4 objectColor = { 1.0f, 1.0f, 0.0f, 1.0f }; // Yellow
    };

    struct DebugVertex
    {
        glm::vec3 position;
        glm::vec4 color;

    };

    struct PushConstants
    {
        glm::mat4 objectMatrix = glm::identity<glm::mat4>();
        glm::vec4 objectColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        glm::vec4 useTexture = { };
    };

    struct MeshVertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textCoord; // UV
    };

    struct CameraConstants
    {
        static constexpr uint32_t binding = 0;

        glm::mat4 projectionMatrix = glm::identity<glm::mat4>();
        glm::mat4 viewMatrix = glm::identity<glm::mat4>();

        glm::vec3 lightPosition = { -12.0f, 23.0f, -40.0f };
        float pad0 = 1.0f;

        glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f };
        float pad1 = 1.0f;

        glm::vec3 lightAttenuation = { 1.0f, 0.0f, 0.0f };
        float pad2 = 1.0f;

        glm::vec3 lightPosition2 = { 479.0f, 23.0f, 50.0f };
        float pad3 = 1.0f;

        glm::vec3 lightColor2 = { 0.031f, 0.161f, 0.035f };
        float pad4 = 1.0f;

        glm::vec3 lightAttenuation2 = { 1.0f, 0.0f, 0.0f };
        float pad5 = 1.0f;
    };


    class WindowHandle;

    class VulkanRenderer : public Brokkr::CoreSubsystem
    {

        std::vector<DebugVertex> cubeVertices =
        {
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        };

        std::vector<uint32_t> cubeIndices = 
        {
            0, 1, 1, 2, 2, 3, 3, 0, // bottom square
            4, 5, 5, 6, 6, 7, 7, 4, // top square
            0, 4, 1, 5, 2, 6, 3, 7  // vertical lines
        };
        
        // TODO: Add Camera Syytem and support for multiple cameras
        Camera m_camera;

        std::unique_ptr<Mesh<Brokkr::DebugVertex>> debugBoxMesh;

        vk::Buffer cameraConstantsBuffer;
        vk::DeviceMemory cameraConstantsMemory;

        GAP311::VulkanWrapper::ShaderUniform cameraUniform;

        vk::PolygonMode meshPolygonMode = vk::PolygonMode::eFill;
        vk::CullModeFlags meshCullMode = vk::CullModeFlagBits::eFront;
        vk::FrontFace meshFrontFace = vk::FrontFace::eCounterClockwise;
        bool meshDepthTestEnable = VK_TRUE;
        vk::CompareOp meshDepthCompareOp = vk::CompareOp::eLess;
        bool meshDepthWriteEnable = VK_TRUE;

        std::shared_ptr<GAP311::VulkanWrapper::GraphicsPipeline> gridPipeline;
        std::shared_ptr<GAP311::VulkanWrapper::GraphicsPipeline> debugBoxPipeline;
        std::shared_ptr<GAP311::VulkanWrapper::GraphicsPipeline> meshPipeline;

        VulkanPipelineManager* m_pPipelineManager = nullptr;

        // Shader library for loading and compiling
        GAP311::VulkanShaderLibrary m_shaderLibrary;

        GAP311::VulkanWrapper m_VulkanContext;   // Vulkan context                                    
        WindowHandle* m_pWindow = nullptr;
        IMGUIManager* m_pImguiManager = nullptr;
        std::queue<RenderObject> m_RenderQueue;


    public:
        CameraConstants cameraConstants;

        explicit VulkanRenderer(CoreSystems* pCoreManager);

        void Init(const char* exePath);    // Initialize Vulkan

        void QueueRenderObject(const RenderObject& renderObject);
        void ProcessFrame();

        void RebuildMeshPipeline();

        GAP311::VulkanWrapper& GetVulkanContext() { return m_VulkanContext; }
        virtual void Destroy() override;
    };
}

template <typename StoredValue, typename Value>
bool CheckIfChanged(StoredValue& val, const Value& newVal)
{
    bool changed = (val != newVal);
    val = newVal;
    return changed;
}

template <typename Iter, typename Value>
int IndexOf(Iter first, const Iter last, const Value& val, int defaultIndex = 0)
{
    auto loc = std::find(first, last, val);
    if (loc == last)
        return defaultIndex;
    return static_cast<int>(std::distance(first, loc));
}
