#pragma once
#include <memory>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Transform.h"
#include "../../CoreSystems/CoreSystems.h"
#include "../../GameEntity/GameEntity.h"
#include "../../Renderer/Vulkan/VulkanRenderer.h"

namespace GAP311
{
    class VulkanWrapper;
}

class IMGUIManager;

namespace Brokkr
{
    class VulkanRenderer;
}

template<typename T>
class Mesh;


class SceneNode : public std::enable_shared_from_this<SceneNode>
{


public:
    std::string name;                                 // Name for the node
    Transform transform;                              // Local transformation
    Brokkr::GameEntity m_pGameObject;                 // Game Object Container of components
    std::weak_ptr<SceneNode> parent;                  // Parent node in the hierarchy
    std::vector<std::shared_ptr<SceneNode>> children; // Child nodes


    std::shared_ptr<Mesh<Brokkr::MeshVertex>> mesh;  // mesh move to componet later
    std::shared_ptr<Texture> texture;

    SceneNode(const std::string& nodeName)
        : name(nodeName)
    {
        //
    }

    // Add child node to the current node
    void AddChild(std::shared_ptr<SceneNode> child)
    {
        child->parent = shared_from_this();
        children.push_back(child);
    }

    // he world transform of this node considering its parent node
    glm::mat4 GetWorldTransform() const
    {

        if (auto p = parent.lock())
        {
            if (p->name == "Root")
            {
                return transform.GetLocalTransform(); // Root node world transform is its local transform
            }

            return transform.GetWorldTransform(p->GetWorldTransform());
        }

        return transform.GetLocalTransform(); // Root node world transform is its local transform
    }
};


class SceneGraph
{
    Assimp::Importer m_importer;
    GAP311::VulkanWrapper& m_VulkanContext;
    std::shared_ptr<SceneNode> m_rootNode;      // Root node of the scene graph

public:

    // For editing scene nodes
    struct SceneNodeDebugInfo
    {
        std::string name;
        std::shared_ptr<SceneNode> pNode;       

        // Constructor for convenience
        SceneNodeDebugInfo(const std::string& nodeName, std::shared_ptr<SceneNode> node)
            : name(nodeName)
            , pNode(node)
        {
            //
        }
    };
    std::vector<SceneNodeDebugInfo> m_debugNodeInfo;

    //std::vector<std::string> m_sceneNodeNames; // TODO: Fix this later proper encap

    SceneGraph(Brokkr::CoreSystems* pEngineSystems)
        : m_VulkanContext(pEngineSystems->GetCoreSystem<Brokkr::VulkanRenderer>()->GetVulkanContext())
        , m_rootNode(std::make_shared<SceneNode>("Root"))
    {
        //
    }

    ~SceneGraph()
    {
        CleanupNode(m_rootNode);
    }

    // Add node to the scene graph
    SceneNode* AddNode(std::shared_ptr<SceneNode> node, const std::string& parentName = "");

    void RemoveNode(const std::string& nodeName);

    std::shared_ptr<SceneNode> FindParentNode(std::shared_ptr<SceneNode> node, const std::string& name);

    std::shared_ptr<SceneNode> ImportMesh(const std::string& sceneName, const std::string& filepath, float uvScaleFactor = 1.0f);

    // Find node by name
    std::shared_ptr<SceneNode> FindNode(const std::string& name);
    std::shared_ptr<SceneNode> FindNode(std::shared_ptr<SceneNode> node, const std::string& name);

    // Render the scene graph TODO: Implement rendering
    void RenderScene(Brokkr::VulkanRenderer* pRenderer);

    // Getter for the World node
    std::shared_ptr<SceneNode> GetRootNode() const;
    void CleanupNode(std::shared_ptr<SceneNode> node);

private:

    // Recursive rendering of all nodes
    static void RenderNode(std::shared_ptr<SceneNode> node, Brokkr::VulkanRenderer* pRenderer);

    void ProcessAssimpNode(const std::string& sceneName, aiNode* assimpNode, const aiScene* scene, std::shared_ptr<SceneNode> parentNode, float uvScaleFactor);
};

