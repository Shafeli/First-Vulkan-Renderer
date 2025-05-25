#pragma once


#include "IMGUIManager/IMGUIManager.h"
#include "SceneManager/Scene.h"

namespace Brokkr
{
    class VulkanRenderer;
}

class MainScene : public Brokkr::Scene
{
    Brokkr::VulkanRenderer* m_pVulkanRenderer = nullptr;
    Brokkr::EntityXMLParser* m_pEntityXmlParser = nullptr;
    IMGUIManager* m_pImguiManager = nullptr;

    SceneNode* m_pSceneRootNode = nullptr;

public:
    explicit MainScene(Brokkr::CoreSystems* pCoreManager);

    void Init() override;
    void Update() override;
    void Exit() override;
    void Enter() override;
    void Destroy() override;
};

