#include "TestingScene.h"

#include <imgui.h>
#include <glm/ext.hpp>

#include "CoreSystems/CoreSystems.h"
#include "Renderer/Vulkan/Mesh.h"
#include "Renderer/Vulkan/VulkanRenderer.h"
#include "XMLManager/XMLManager.h"
#include "SceneManager/SceneManager.h"
#include "XMLManager/Parsers/EntityXMLParser/EntityXMLParser.h"

TestingScene::TestingScene(Brokkr::CoreSystems* pCoreManager)
    : Scene(pCoreManager)
{
    m_pVulkanRenderer = m_pCoreSystems->GetCoreSystem<Brokkr::VulkanRenderer>();
    m_pImguiManager = m_pCoreSystems->GetCoreSystem<IMGUIManager>();
    m_pEntityXmlParser = m_pCoreSystems->GetCoreSystem<Brokkr::XMLManager>()->GetParser<Brokkr::EntityXMLParser>();

}

void TestingScene::Init()
{
    auto sphereNode = m_sceneGraph.ImportMesh("cube", "Assets/cube.glb", 16.0f);
    sphereNode->transform.position = glm::vec3(0.0, 0.0, -2.0);
    sphereNode->transform.scale = glm::vec3(10, 0, 7);
    sphereNode->texture = std::make_shared<Texture>("Assets/floor.png", m_pVulkanRenderer->GetVulkanContext());

    auto sphereNode2 = m_sceneGraph.ImportMesh("icosphere", "Assets/icosphere.glb");
    sphereNode2->transform.position = glm::vec3(3.0, 1.0, 3.0);
    sphereNode2->transform.scale = glm::vec3(1, 1, 1);
    //sphereNode2->texture = std::make_shared<Texture>("Assets/floor.png", m_pVulkanRenderer->GetVulkanContext());

    auto sphereNode4 = m_sceneGraph.ImportMesh("rubber_duck_toy", "Assets/rubber_duck_toy.gltf");
    sphereNode4->transform.position = glm::vec3(-3.0, 0, 1);
    sphereNode4->transform.rotation = glm::vec3(0, 234.0, 1);
    sphereNode4->transform.scale = glm::vec3(10, 10, 10);
    sphereNode4->texture = std::make_shared<Texture>("Assets/rubber_duck_toy_diff.png", m_pVulkanRenderer->GetVulkanContext());

}

void TestingScene::Update()
{
    // Render the scene with the Vulkan renderer
    m_sceneGraph.RenderScene(m_pVulkanRenderer);
    m_pImguiManager->ImGuiOnEvent(*m_pCoreSystems->m_pFrameEvent);


    // Enqueue the UI rendering Debug Window
    m_pImguiManager->EnqueueUI([this]()
        {
            if (ImGui::Begin("Scene Debug Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                if (ImGui::Button("MainScene"))
                {
                    auto pSceneManager = m_pCoreSystems->GetCoreSystem<Brokkr::SceneManager>();
                    pSceneManager->SetActiveState("MainScene");
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


    m_pVulkanRenderer->ProcessFrame();
}


void TestingScene::Exit()
{

}

void TestingScene::Enter()
{

}

void TestingScene::Destroy()
{

}

