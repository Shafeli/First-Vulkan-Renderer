#include "IMGUIManager.h"
#include <imgui.h>
#include <SDL_log.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_vulkan.h>

#include "../../gap-311/VulkanWrapper.hpp"
#include "../WindowHandle/WindowHandle.h"

#include <vulkan/vulkan_handles.hpp>

#include "../Renderer/Vulkan/VulkanRenderer.h"

namespace Brokkr
{
    class VulkanRenderer;
}

void IMGUIManager::EnqueueUI(std::function<void()> uiWindowCallBack)
{
	m_uiQueue.push((uiWindowCallBack));
}

void IMGUIManager::ProcessUI()
{
	while (!m_uiQueue.empty())
	{
		auto fn = m_uiQueue.front();

		fn();                // each lambda calls ImGui::Begin()/…ImGui::End()

		m_uiQueue.pop();
	}
}

void IMGUIManager::RenderUI(vk::CommandBuffer& commands)
{
	ImGuiRender(commands);
}

bool IMGUIManager::ImGuiInitialize(Brokkr::WindowHandle* pWindow, GAP311::VulkanWrapper& vulkan)
{

	IMGUI_CHECKVERSION();
	if (!ImGui::CreateContext())
	{
		SDL_Log("Failed to create ImGui context!");
		return false;
	}

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForVulkan(pWindow->GetWindow());
	ImGui_ImplVulkan_InitInfo initInfo = {};
	initInfo.Instance = vulkan.GetInstance();
	initInfo.PhysicalDevice = vulkan.GetPhysicalDevice();
	initInfo.Device = vulkan.GetDevice();
	initInfo.QueueFamily = vulkan.GetGraphicsQueueIndex();
	initInfo.Queue = vulkan.GetGraphicsQueue();
	initInfo.PipelineCache = vulkan.GetPipelineCache();
	initInfo.DescriptorPool = vulkan.GetDescriptorPool();
	initInfo.RenderPass = vulkan.GetDisplayRenderPass();
	initInfo.Subpass = 0;
	initInfo.MinImageCount = 2;
	initInfo.ImageCount = vulkan.GetSwapchain().image_count;
	initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	initInfo.Allocator = nullptr;
	initInfo.CheckVkResultFn = nullptr;

	if (!ImGui_ImplVulkan_Init(&initInfo))
	{
		SDL_Log("Failed to init ImGui Vulkan backend.");
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
		return false;
	}

	return true;
}

void IMGUIManager::ImGuiShutdown()
{
	if (isAlive)
	{
	    auto vulkan = m_pCoreManager->GetCoreSystem<Brokkr::VulkanRenderer>();
	    auto vkGraphicQueue = vulkan->GetVulkanContext().GetGraphicsQueue();

	    vkQueueWaitIdle(vkGraphicQueue);

	    ImGui_ImplVulkan_Shutdown();
	    ImGui_ImplSDL2_Shutdown();
	    ImGui::DestroyContext();
		isAlive = false;
	}
}

void IMGUIManager::ImGuiNewFrame()
{

	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void IMGUIManager::ImGuiRender(vk::CommandBuffer& commands)
{
	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commands);
}

void IMGUIManager::Destroy()
{
    ImGuiShutdown();
}

void IMGUIManager::ImGuiOnEvent(const SDL_Event& event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}
