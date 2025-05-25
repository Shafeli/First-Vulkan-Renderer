#pragma once
#include <functional>
#include <queue>
#include <SDL_log.h>
#include <SDL2/SDL_events.h>
#include <vulkan/vulkan_core.h>


#include "../CoreSystems/CoreSystems.h"


namespace vk
{
    class CommandBuffer;
}

namespace Brokkr
{
    class WindowHandle;
}

namespace GAP311
{
    class VulkanWrapper;
}

class IMGUIManager : public Brokkr::CoreSubsystem
{
    // TODO THIS IS A TEMP/ Forever forgotten fix
    bool isAlive = true;
    std::queue<std::function<void()>> m_uiQueue;
public:
    explicit IMGUIManager(Brokkr::CoreSystems* pCoreManager, Brokkr::WindowHandle* pWindow, GAP311::VulkanWrapper& vulkan)
        : CoreSubsystem(pCoreManager)
    {
        //
        if (!ImGuiInitialize(pWindow, vulkan))
        {
            SDL_Log("Failed initializing ImGui.");
        }
    }

    // Enqueue a IMGUI window to be processed during rendering process
    void EnqueueUI(std::function<void()> uiWindowCallBack);

    // Render process calls to .... process all UI Windows queue this frame
    void ProcessUI();
    void RenderUI(vk::CommandBuffer& commands);

    bool ImGuiInitialize(Brokkr::WindowHandle* pWindow, GAP311::VulkanWrapper& vulkan);
    void ImGuiShutdown();
    void ImGuiNewFrame();
    void ImGuiOnEvent(const SDL_Event& event);
    void ImGuiRender(vk::CommandBuffer& commands);


    virtual void Destroy() override;
};

