
#include "TestingScene.h"
#include "MainScene.h"

#include "../../gap-311/VulkanWrapper.hpp"
#include "CoreSystems/CoreSystems.h"
#include "Renderer/Vulkan/VulkanRenderer.h"
#include "SceneManager/SceneManager.h"
#include "WindowHandle/WindowHandle.h"
#include "IMGUIManager/IMGUIManager.h"


// https://www.youtube.com/watch?v=9RHGLZLUuwc&ab_channel=SebastianLague Coding Adventure: Compute Shaders
// https://www.youtube.com/watch?v=g9FoZcEQlbA&ab_channel=Vulkan Vulkanised 2023: Mesh shading best practices
// https://www.youtube.com/watch?v=Pj1P0zV4zDI&t=29s&ab_channel=frozein Learning VULKAN by Rendering a GALAXY
// https://www.youtube.com/watch?v=BwCqRqqbB1Y&ab_channel=TheCherno Renderer API Abstraction | Game Engine series

int main(int argc, char* argv[])
{

	const char* appName = argv[0];

	Brokkr::CoreSystems CoreSystemsManager;

	CoreSystemsManager.Init("Vulkan Renderer", 1920, 1080);
	const auto pSceneManager = CoreSystemsManager.GetCoreSystem<Brokkr::SceneManager>();

    const auto pWindow = CoreSystemsManager.GetCoreSystem<Brokkr::WindowHandle>();
	pWindow->InitWindow("Vulkan Renderer", 1920, 1080);

    const auto pVulkanRenderer = CoreSystemsManager.AddCoreSystem<Brokkr::VulkanRenderer>();
	pVulkanRenderer->Init(std::filesystem::path(argv[0]).parent_path().generic_string().c_str());

	auto pImguiManager = CoreSystemsManager.AddCoreSystem<IMGUIManager>(pWindow, pVulkanRenderer->GetVulkanContext());



	pSceneManager->AddState("Testing", std::make_unique<TestingScene>(&CoreSystemsManager));
	pSceneManager->AddState("MainScene", std::make_unique<MainScene>(&CoreSystemsManager));


	pSceneManager->SetActiveState("MainScene");

	CoreSystemsManager.Run();

	
	// Shutting down Imgui before everything else
	pImguiManager->Destroy();

	CoreSystemsManager.Destroy();
	return 0;
}
