#include "MainScene.h"

#include <imgui.h>
#include <glm/ext.hpp>

#include "CoreSystems/CoreSystems.h"
#include "Renderer/Vulkan/Mesh.h"
#include "Renderer/Vulkan/VulkanRenderer.h"
#include "XMLManager/XMLManager.h"
#include "SceneManager/SceneManager.h"
#include "XMLManager/Parsers/EntityXMLParser/EntityXMLParser.h"

MainScene::MainScene(Brokkr::CoreSystems* pCoreManager)
    : Scene(pCoreManager)
{
    m_pVulkanRenderer = m_pCoreSystems->GetCoreSystem<Brokkr::VulkanRenderer>();
    m_pImguiManager = m_pCoreSystems->GetCoreSystem<IMGUIManager>();
    m_pEntityXmlParser = m_pCoreSystems->GetCoreSystem<Brokkr::XMLManager>()->GetParser<Brokkr::EntityXMLParser>();

}

void MainScene::Init()
{

    auto sphereNod7 = m_sceneGraph.ImportMesh("Lightsaber Lighter MK4", "Assets/Lightsaber Lighter MK4.stl");
    sphereNod7->transform.position = glm::vec3(6.0, 5.0, 3.0);
    sphereNod7->transform.scale = glm::vec3(.025, .025, .025);
    //sphereNod7->texture = std::make_shared<Texture>("Assets/floor.png", m_pVulkanRenderer->GetVulkanContext());


    auto sphereNode = m_sceneGraph.ImportMesh("cube", "Assets/cube.glb", 16.0f);
    sphereNode->transform.position = glm::vec3(0.0, 0.0, -2.0);
    sphereNode->transform.scale = glm::vec3(10, 0, 7);
    sphereNode->texture = std::make_shared<Texture>("Assets/floor.png", m_pVulkanRenderer->GetVulkanContext());

    auto sphereNode4 = m_sceneGraph.ImportMesh("rubber_duck_toy", "Assets/rubber_duck_toy.gltf");
    sphereNode4->transform.position = glm::vec3(-3.0, 0, 1);
    sphereNode4->transform.rotation = glm::vec3(0, 234.0, 1);
    sphereNode4->transform.scale = glm::vec3(10, 10, 10);
    sphereNode4->texture = std::make_shared<Texture>("Assets/rubber_duck_toy_diff.png", m_pVulkanRenderer->GetVulkanContext());

    auto sphereNode5 = m_sceneGraph.ImportMesh("rubber_duck_toy_00", "Assets/rubber_duck_toy.gltf");
    sphereNode5->transform.position = glm::vec3(3.0, 0, 1);
    sphereNode5->transform.rotation = glm::vec3(0, 234.0, 1);
    sphereNode5->transform.scale = glm::vec3(10, 10, 10);
    sphereNode5->texture = std::make_shared<Texture>("Assets/rubber_duck_toy_diff.png", m_pVulkanRenderer->GetVulkanContext());

    auto sphereNode6 = m_sceneGraph.ImportMesh("rubber_duck_toy_01", "Assets/rubber_duck_toy.gltf");
    sphereNode6->transform.position = glm::vec3(-3.0, 0, 10);
    sphereNode6->transform.rotation = glm::vec3(0, 234.0, 1);
    sphereNode6->transform.scale = glm::vec3(10, 10, 10);
    sphereNode6->texture = std::make_shared<Texture>("Assets/rubber_duck_toy_diff.png", m_pVulkanRenderer->GetVulkanContext());

    auto sphereNode2 = m_sceneGraph.ImportMesh("icosphere", "Assets/icosphere.glb");
    sphereNode2->transform.position = glm::vec3(3.0, 5.0, 3.0);
    sphereNode2->transform.scale = glm::vec3(1, 1, 1);
    sphereNode2->texture = std::make_shared<Texture>("Assets/floor.png", m_pVulkanRenderer->GetVulkanContext());

    auto sphereNode3 = m_sceneGraph.ImportMesh("icosphere_00", "Assets/icosphere.glb");
    sphereNode3->transform.position = glm::vec3(-3.0, 5.0, 3.0);
    sphereNode3->transform.scale = glm::vec3(1, 1, 1);
    //sphereNode2->texture = std::make_shared<Texture>("Assets/floor.png", m_pVulkanRenderer->GetVulkanContext());

    auto sphereNod73 = m_sceneGraph.ImportMesh("toutatis", "Assets/toutatis.stl");
    sphereNod73->transform.position = glm::vec3(-6.0, 5.0, 3.0);
    sphereNod73->transform.scale = glm::vec3(1, 1, 1);
    sphereNod73->texture = std::make_shared<Texture>("Assets/floor.png", m_pVulkanRenderer->GetVulkanContext());

}

void MainScene::Update()
{
    // Render the scene with the Vulkan renderer
    m_sceneGraph.RenderScene(m_pVulkanRenderer);
    m_pImguiManager->ImGuiOnEvent(*m_pCoreSystems->m_pFrameEvent);



    // Enqueue the UI rendering Debug Window
    m_pImguiManager->EnqueueUI([this]()
        {

            if (ImGui::Begin("MainScene Debug Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {

                if (ImGui::Button("TestingScene_00"))
                {
                    auto pSceneManager = m_pCoreSystems->GetCoreSystem<Brokkr::SceneManager>();
                    pSceneManager->SetActiveState("Testing");
                }

                ImGui::SeparatorText("Scene Object List");

                for (auto& nodeInfo : m_sceneGraph.m_debugNodeInfo)
                {
                    if (ImGui::TreeNode(nodeInfo.name.c_str()))
                    {
                        // Group related properties together
                        ImGui::BeginGroup();
                        ImGui::Text("Transform:");
                        // Add spacing
                        ImGui::Dummy(ImVec2(0.0f, 1.0f));

                        ImGui::Indent();

                        ImGui::DragFloat3("Position", glm::value_ptr(nodeInfo.pNode->transform.position));
                        ImGui::DragFloat3("Rotation", glm::value_ptr(nodeInfo.pNode->transform.rotation));
                        ImGui::DragFloat3("Scale", glm::value_ptr(nodeInfo.pNode->transform.scale));

                        ImGui::Unindent();

                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        ImGui::Text("Mesh Details:");

                        // Add spacing
                        ImGui::Dummy(ImVec2(0.0f, 1.0f));
                        ImGui::Indent();

                        ImGui::Text("Vertices: %d", nodeInfo.pNode->mesh->vertexCount);
                        ImGui::Text("Triangles: %d", nodeInfo.pNode->mesh->indexCount / 3);

                        ImGui::Unindent();

                        ImGui::EndGroup();

                        // Add spacing
                        ImGui::Dummy(ImVec2(0.0f, 5.0f));

                        ImGui::TreePop();
                    }
                }
            }
            ImGui::End();
        });


    // Simple animation
    for (auto& nodeInfo : m_sceneGraph.m_debugNodeInfo)
    {
        if (nodeInfo.pNode->name == "icosphere")
        {
            float angle = glm::radians(90.0f) * m_pCoreSystems->GetDeltaTime()->GetSeconds(); // 90d per second
            nodeInfo.pNode->transform.rotation.y += angle;
        }

        if (nodeInfo.pNode->name == "icosphere_00")
        {
            float angle = glm::radians(-90.0f) * m_pCoreSystems->GetDeltaTime()->GetSeconds(); // 90d per second
            nodeInfo.pNode->transform.rotation.y += angle;
        }

        if (nodeInfo.pNode->name == "rubber_duck_toy")
        {
            float angle = glm::radians(1080.0f) * m_pCoreSystems->GetDeltaTime()->GetSeconds(); // 90d per second
            nodeInfo.pNode->transform.rotation.x += angle;
        }

        if (nodeInfo.pNode->name == "rubber_duck_toy_00")
        {
            float angle = glm::radians(-1080.0f) * m_pCoreSystems->GetDeltaTime()->GetSeconds(); // 90d per second
            nodeInfo.pNode->transform.rotation.x += angle;
        }

        if (nodeInfo.pNode->name == "Lightsaber Lighter MK4")
        {
            float angle = glm::radians(-1080.0f) * m_pCoreSystems->GetDeltaTime()->GetSeconds(); // 90d per second
            nodeInfo.pNode->transform.rotation.z += angle;
        }

        if (nodeInfo.pNode->name == "toutatis")
        {
            float angle = glm::radians(1080.0f) * m_pCoreSystems->GetDeltaTime()->GetSeconds(); // 90d per second
            nodeInfo.pNode->transform.rotation.z += angle;
        }
    }

    m_pVulkanRenderer->ProcessFrame();
}


void MainScene::Exit()
{

}

void MainScene::Enter()
{

}

void MainScene::Destroy()
{

}
