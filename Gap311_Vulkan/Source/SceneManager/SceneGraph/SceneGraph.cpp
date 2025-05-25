#include "SceneGraph.h"

#include <iostream>
#include <assimp/postprocess.h>
#include <memory>
#include "../../Renderer/Vulkan/VulkanRenderer.h"
#include "../../Renderer/Vulkan/Mesh.h"
#include <assimp/scene.h>

SceneNode* SceneGraph::AddNode(std::shared_ptr<SceneNode> node, const std::string& parentName)
{
    if (parentName.empty()) 
    {
        m_rootNode->AddChild(node);
        m_debugNodeInfo.emplace_back(node->name, node);
    }
    else 
    {
        std::shared_ptr<SceneNode> parent = FindNode(parentName);
        if (parent) 
        {
            parent->AddChild(node);
            m_debugNodeInfo.emplace_back(node->name, node);
        }
    }

    return node.get();
}

void SceneGraph::RemoveNode(const std::string& nodeName)
{
    auto parentNode = FindParentNode(m_rootNode, nodeName);
    if (parentNode)
    {
        // Remove all matching children with the given name
        auto& children = parentNode->children;
        auto it = children.begin();
        while (it != children.end())
        {
            if ((*it)->name == nodeName)
            {
                it = children.erase(it); // Safely erase while looping
            }
            else
            {
                ++it;
            }
        }
    }
}

std::shared_ptr<SceneNode> SceneGraph::FindParentNode(std::shared_ptr<SceneNode> node, const std::string& name)
{
    for (auto& child : node->children) 
    {
        if (child->name == name) 
        {
            return node;
        }

        auto parent = FindParentNode(child, name);
        if (parent) 
        {
            return parent;
        }
    }
    return nullptr; // Node not found
}

std::shared_ptr<SceneNode> SceneGraph::ImportMesh(const std::string& sceneName, const std::string& filepath, float uvScaleFactor)
{
    // Load the scene using Assimp
    const aiScene* scene = m_importer.ReadFile(filepath,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ConvertToLeftHanded);

    // Check if the scene was loaded
    if (!scene || !scene->mRootNode) 
    {
        std::cerr << "Assimp failed to load model: " << m_importer.GetErrorString() << std::endl;
        return nullptr;
    }

    // auto leadingObject = scene->mRootNode->mName;
    // Start processing from the root node
    std::string extension = std::filesystem::path(filepath).extension().string();  // returns ".somthing"
    if (extension == ".stl") // stls seem to have this first parent node and empty so this sovles a crash and a phantom object in the scene view
    {
        ProcessAssimpNode(sceneName, scene->mRootNode->mChildren[0], scene, m_rootNode, uvScaleFactor);
        return FindNode(sceneName);
    }

    ProcessAssimpNode(sceneName, scene->mRootNode, scene, m_rootNode, uvScaleFactor);
    return FindNode(sceneName);
}


void SceneGraph::ProcessAssimpNode(const std::string& sceneName, aiNode* assimpNode, const aiScene* scene, std::shared_ptr<SceneNode> parentNode, float uvScaleFactor)
{
    // Create a new SceneNode with the name from the Assimp node
    const auto newNode = std::make_shared<SceneNode>(sceneName);
    m_debugNodeInfo.emplace_back(sceneName, newNode );

    newNode->texture = std::make_shared<Texture>(m_VulkanContext);

    // Decompose the transformation matrix from Assimp and apply it to the new node
    newNode->transform.DecomposeFromMatrix(assimpNode->mTransformation);

    // Add the new node as a child of the parent node
    parentNode->AddChild(newNode);

    // Process all meshes associated with this node
    for (unsigned int i = 0; i < assimpNode->mNumMeshes; ++i)
    {
        unsigned int meshIndex = assimpNode->mMeshes[i];
        aiMesh* assimpMesh = scene->mMeshes[meshIndex];

        // Create a new Mesh instance
        auto mesh = std::make_shared<Mesh<Brokkr::MeshVertex>>(m_VulkanContext);

        // Populate the Mesh with vertex and index data
        mesh->vertexCount = assimpMesh->mNumVertices;
        mesh->indexCount = 0;

        // Extract vertex positions, normals, etc.
        for (unsigned int v = 0; v < assimpMesh->mNumVertices; ++v)
        {
            Brokkr::MeshVertex vertex;
            vertex.position = glm::vec3(
                assimpMesh->mVertices[v].x,
                assimpMesh->mVertices[v].y,
                assimpMesh->mVertices[v].z
            );

            if (assimpMesh->HasNormals())
            {
                vertex.normal = glm::vec3(
                    assimpMesh->mNormals[v].x,
                    assimpMesh->mNormals[v].y,
                    assimpMesh->mNormals[v].z
                );
            }

            if (assimpMesh->HasTextureCoords(0))
            {
                vertex.textCoord = glm::vec2(
                    assimpMesh->mTextureCoords[0][v].x * uvScaleFactor,
                    assimpMesh->mTextureCoords[0][v].y * uvScaleFactor

                );
            }
            else
            {
                vertex.textCoord = glm::vec2(0.0f, 0.0f); // default values
            }

            // TODO: more attributes 

            mesh->vertices.push_back(vertex);
        }

        // Extract indices
        for (unsigned int f = 0; f < assimpMesh->mNumFaces; ++f)
        {
            aiFace& face = assimpMesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
            {
                mesh->indices.push_back(face.mIndices[j]);
                ++mesh->indexCount;
            }
        }

        aiColor4D rgba;
        const auto& material = scene->mMaterials[assimpMesh->mMaterialIndex];
        material->Get(AI_MATKEY_BASE_COLOR, rgba);
        mesh->color = glm::vec4{ rgba.r, rgba.g, rgba.b, rgba.a };

        // Create Vulkan buffers for the mesh
        mesh->Load();

        // Assign the mesh to the new node
        newNode->mesh = mesh;
    }

    // Recursively process all child nodes
    for (unsigned int i = 0; i < assimpNode->mNumChildren; ++i)
    {
        ProcessAssimpNode(assimpNode->mName.C_Str(), assimpNode->mChildren[i], scene, newNode, uvScaleFactor);
    }
}



std::shared_ptr<SceneNode> SceneGraph::FindNode(const std::string& name)
{
    return FindNode(m_rootNode, name);
}

std::shared_ptr<SceneNode> SceneGraph::FindNode(std::shared_ptr<SceneNode> node, const std::string& name)
{
    if (node->name == name) 
    {
        return node;
    }

    for (auto& child : node->children) 
    {
        auto foundNode = FindNode(child, name);
        if (foundNode) 
        {
            return foundNode;
        }
    }

    return nullptr; // Node not found
}

void SceneGraph::RenderScene(Brokkr::VulkanRenderer* pRenderer)
{
    RenderNode(m_rootNode, pRenderer);
}

void SceneGraph::RenderNode(std::shared_ptr<SceneNode> node, Brokkr::VulkanRenderer* pRenderer)
{
    if (node->mesh) 
    {
       // std::cout << "Rendering object: " << node->name << std::endl;

        // Render the mesh with its world transform
       const glm::mat4 worldTransform = node->GetWorldTransform();

        // Pass worldTransform and mesh ect.. to the rendering system
        RenderObject renderObject(worldTransform, node->mesh, node->texture);
        pRenderer->QueueRenderObject(renderObject);
    }

    // Render all children
    for (auto& child : node->children) 
    {
        RenderNode(child, pRenderer);
    }
}

std::shared_ptr<SceneNode> SceneGraph::GetRootNode() const
{
    return m_rootNode;
}

void SceneGraph::CleanupNode(std::shared_ptr<SceneNode> node)
{
    if (node->mesh)
    {
        node->mesh->Destroy();
        node->mesh.reset();
        node->texture->~Texture();
    }

    for (auto child : node->children)
    {
        CleanupNode(child);
    }
}
