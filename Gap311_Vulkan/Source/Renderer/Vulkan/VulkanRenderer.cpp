#include "VulkanRenderer.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <imgui.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <SDL.h>
#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Mesh.h"
#include "VulkanBufferManager.h"
#include "VulkanPipelineManager.h"
#include "../../CoreSystems/CoreSystems.h"
#include "../../gap-311/VulkanShaderLibrary.hpp"
#include "../../gap-311/VulkanWrapper.hpp"
#include "../../WindowHandle/WindowHandle.h"
#include "../../IMGUIManager/IMGUIManager.h"

namespace Brokkr
{


    VulkanRenderer::VulkanRenderer(CoreSystems* pCoreManager)
        : CoreSubsystem(pCoreManager)
    {
        pCoreManager->AddCoreSystem<VulkanBufferManager>(m_VulkanContext);
    }

	void VulkanRenderer::Init(const char* exePath)
	{
		m_pWindow = m_pCoreManager->GetCoreSystem<WindowHandle>();
        m_pImguiManager = m_pCoreManager->GetCoreSystem<IMGUIManager>();

		GAP311::VulkanWrapper::ConfigOptions options{};
		options.configureInstanceFunc = [](vkb::InstanceBuilder& builder)
			{
				builder.require_api_version(1, 3);
			};
		options.configureDeviceFunc = [](vkb::PhysicalDeviceSelector& selector)
			{
				selector.set_required_features(
					vk::PhysicalDeviceFeatures()
					.setFillModeNonSolid(true) // support wireframe
				);
			};
		options.debugLogFunc = GAP311::VulkanWrapper::VulkanLogToSDL();
		if (!m_VulkanContext.Initialize(exePath, GAP311::VulkanWrapper::CreateVulkanSurfaceSDL(m_pWindow->GetWindow()), options))
		{
			SDL_Log("Failed initializing VulkanWrapper.");
		}

		GAP311::VulkanShaderLibrary::ConfigOptions shaderOptions{};
		shaderOptions.logMessage = [](const char* msg)
			{
				SDL_Log("ShaderLibrary: %s", msg);
			};
		shaderOptions.searchDirs = {
			std::filesystem::current_path().string(), // working dir
			std::filesystem::path(exePath).parent_path().string(), // exe dir
			std::filesystem::relative(std::filesystem::path(__FILE__).parent_path()).string(), // source file dir
		};
		if (!m_shaderLibrary.Initialize(m_VulkanContext.GetDevice(), shaderOptions))
		{
			SDL_Log("Failed initializing VulkanShaderLibrary.");
			m_VulkanContext.Shutdown();
		}

		int windowWidth = 0, windowHeight = 0;
		SDL_GetWindowSize(m_pWindow->GetWindow(), &windowWidth, &windowHeight);

		cameraConstants.viewMatrix = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 5.0f), // Camera position
			glm::vec3(0.0f, 0.0f, 0.0f), // Look-at point
			glm::vec3(0.0f, 1.0f, 0.0f)  // Up direction
		);

		cameraConstants.projectionMatrix = glm::perspective(
			glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.1f, 512.0f);

		cameraConstants.projectionMatrix[1][1] *= -1.0f;

		cameraConstantsBuffer = m_VulkanContext.CreateUniformBuffer(sizeof(cameraConstants));

		cameraUniform = m_VulkanContext.CreateUniformForBuffer(CameraConstants::binding, cameraConstantsBuffer, sizeof(cameraConstants));

		cameraConstantsMemory = m_VulkanContext.GetBufferMemoryHandle(cameraConstantsBuffer);

		debugBoxMesh = std::make_unique<Mesh<Brokkr::DebugVertex>>(m_VulkanContext);

		for (DebugVertex debugVertex : cubeVertices)
		{
			debugBoxMesh->vertices.push_back(debugVertex);
		}
		for (uint32_t cubeIndex : cubeIndices)
		{
			debugBoxMesh->indices.push_back(cubeIndex);
		}
		debugBoxMesh->Load();

		debugBoxPipeline = m_VulkanContext.CreatePipeline(
			{
			    .vertexBindings =
				{
					vk::VertexInputBindingDescription()
						.setBinding(0)
						.setInputRate(vk::VertexInputRate::eVertex)
						.setStride(sizeof(DebugVertex)),
				},
				.vertexAttributes =
				{
					vk::VertexInputAttributeDescription()
						.setLocation(0)
						.setBinding(0)
						.setFormat(vk::Format::eR32G32B32Sfloat)
						.setOffset(offsetof(DebugVertex, DebugVertex::position)),
					vk::VertexInputAttributeDescription()
						.setLocation(1)
						.setBinding(0)
						.setFormat(vk::Format::eR32G32B32A32Sfloat)
						.setOffset(offsetof(DebugVertex, DebugVertex::color)),
				},

				 .topology = vk::PrimitiveTopology::eLineList,

				.shaderPushConstants =
				{
					vk::PushConstantRange()
						.setOffset(0)
						.setSize(sizeof(DebugBoxPushConstants))
						.setStageFlags(vk::ShaderStageFlagBits::eAllGraphics)
				},
				.shaderUniforms =
				{
					cameraUniform,
				},
				.shaderStages =
				{
					vk::PipelineShaderStageCreateInfo()
						.setStage(vk::ShaderStageFlagBits::eVertex)
						.setPName("main")
						.setModule(m_shaderLibrary.GetModule("debugBox.vert")),
					vk::PipelineShaderStageCreateInfo()
						.setStage(vk::ShaderStageFlagBits::eFragment)
						.setPName("main")
						.setModule(m_shaderLibrary.GetModule("debugBox.frag"))
				},
				.polygonMode = vk::PolygonMode::eLine,
				.cullMode = vk::CullModeFlagBits::eNone,
				.frontFace = vk::FrontFace::eCounterClockwise,
				.depthTestEnable = true,
				.depthCompareOp = vk::CompareOp::eLess,
				.depthWriteEnable = false,
				//.colorBlendStates = {
				//	vk::PipelineColorBlendAttachmentState()
				//		.setBlendEnable(false)
				//		.setColorBlendOp(vk::BlendOp::eAdd)
				//		.setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
				//		.setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
				//		.setAlphaBlendOp(vk::BlendOp::eAdd)
				//		.setSrcAlphaBlendFactor(vk::BlendFactor::eSrcAlpha)
				//		.setDstAlphaBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
				//		.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
				//},
			});


		gridPipeline = m_VulkanContext.CreatePipeline(
			{
				.shaderPushConstants =
				{
					vk::PushConstantRange()
						.setOffset(0)
						.setSize(sizeof(PushConstants))
						.setStageFlags(vk::ShaderStageFlagBits::eAllGraphics)
				},
				.shaderUniforms =
				{
					cameraUniform,
				},
				.shaderStages =
				{
					vk::PipelineShaderStageCreateInfo()
						.setStage(vk::ShaderStageFlagBits::eVertex)
						.setPName("main")
						.setModule(m_shaderLibrary.GetModule("grid.vert")),
					vk::PipelineShaderStageCreateInfo()
						.setStage(vk::ShaderStageFlagBits::eFragment)
						.setPName("main")
						.setModule(m_shaderLibrary.GetModule("grid.frag"))
				},
				.polygonMode = vk::PolygonMode::eFill,
				.cullMode = vk::CullModeFlagBits::eNone,
				.frontFace = vk::FrontFace::eCounterClockwise,
				.depthTestEnable = true,
				.depthCompareOp = vk::CompareOp::eLess,
				.depthWriteEnable = false,
				.colorBlendStates = {
					vk::PipelineColorBlendAttachmentState()
						.setBlendEnable(true)
						.setColorBlendOp(vk::BlendOp::eAdd)
						.setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
						.setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
						.setAlphaBlendOp(vk::BlendOp::eAdd)
						.setSrcAlphaBlendFactor(vk::BlendFactor::eSrcAlpha)
						.setDstAlphaBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
						.setColorWriteMask(vk::ColorComponentFlagBits::eA | vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB)
				},
			});
		RebuildMeshPipeline();

    }

    void VulkanRenderer::QueueRenderObject(const RenderObject& renderObject)
    {
        m_RenderQueue.push(renderObject);
    }

    void VulkanRenderer::ProcessFrame()
    {
        vk::ClearValue clearValues[] =
        {
            vk::ClearColorValue(0.00f, 0.00f, 0.00f, 0.0f),
            vk::ClearDepthStencilValue(1.0f, 0),
        };

        if (m_pImguiManager == nullptr)
            m_pImguiManager = m_pCoreManager->GetCoreSystem<IMGUIManager>();

		/////////////////////////////////////////////////////////////////////////
        // Animate the light
		static float totalTime = 0.0f;
		totalTime += m_pCoreManager->GetDeltaTime()->GetSeconds();
		glm::vec3 centerPosition(0.0f, -9.0f, 0.0f);
		float speed = 0.5f;
		float radius = 10.0f;
		float angle = totalTime * speed;

		glm::vec3 mixedPos = centerPosition + glm::vec3(cos(angle) * radius, 0.0f, sin(angle) * radius);
		this->cameraConstants.lightPosition.x = mixedPos.x;
		this->cameraConstants.lightPosition.y = mixedPos.y;
		this->cameraConstants.lightPosition.z = mixedPos.z;

        /////////////////////////////////////////////////////////////////////////

		m_pImguiManager->ImGuiNewFrame();

        m_camera.CameraUpdate(m_pCoreManager->GetDeltaTime()->GetSeconds());

        m_camera.CameraGetViewMatrix(&m_camera, &cameraConstants.viewMatrix);

        /////////////////////////////////////////////////////////////////////////
        // TODO: Refactor and move this out to clean up process frame

        m_pImguiManager->EnqueueUI([this]()
			{
				if (ImGui::Begin("Renderer Debug Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::SeparatorText("Mesh Pipeline");

					bool rebuildNeeded = false;

					static const char* polygonModeNames[] = { "Fill", "Line" };
					static const std::array polygonModeValues = { vk::PolygonMode::eFill, vk::PolygonMode::eLine };
					int polygonModeIndex = IndexOf(polygonModeValues.begin(), polygonModeValues.end(), this->meshPolygonMode);
					ImGui::Combo("Polygon Mode", &polygonModeIndex, polygonModeNames, IM_ARRAYSIZE(polygonModeNames));
					rebuildNeeded |= CheckIfChanged(this->meshPolygonMode, polygonModeValues[polygonModeIndex]);

					static const char* cullModeNames[] = { "None", "Front", "Back", "Front and Back" };
					static const std::array cullModeValues = { vk::CullModeFlagBits::eNone, vk::CullModeFlagBits::eFront, vk::CullModeFlagBits::eBack, vk::CullModeFlagBits::eFrontAndBack };
					int cullModeIndex = IndexOf(cullModeValues.begin(), cullModeValues.end(), this->meshCullMode);
					ImGui::Combo("Cull Mode", &cullModeIndex, cullModeNames, IM_ARRAYSIZE(cullModeNames));
					rebuildNeeded |= CheckIfChanged(this->meshCullMode, cullModeValues[cullModeIndex]);

					static const char* frontFaceNames[] = { "Clockwise", "Counter Clockwise" };
					static const std::array frontFaceValues = { vk::FrontFace::eClockwise, vk::FrontFace::eCounterClockwise };
					int frontFaceIndex = IndexOf(frontFaceValues.begin(), frontFaceValues.end(), this->meshFrontFace);
					ImGui::Combo("Front Face", &frontFaceIndex, frontFaceNames, IM_ARRAYSIZE(frontFaceNames));
					rebuildNeeded |= CheckIfChanged(this->meshFrontFace, frontFaceValues[frontFaceIndex]);

					bool depthTestEnable = this->meshDepthTestEnable;
					ImGui::Checkbox("Depth Test Enable", &depthTestEnable);
					rebuildNeeded |= CheckIfChanged(this->meshDepthTestEnable, depthTestEnable);

					bool depthWriteEnable = this->meshDepthWriteEnable;
					ImGui::Checkbox("Depth Write Enable", &depthWriteEnable);
					rebuildNeeded |= CheckIfChanged(this->meshDepthWriteEnable, depthWriteEnable);

					static const char* depthCompareOpNames[] = { "Never", "Always", "Equal", "Not Equal", "Greater", "Greater or Equal", "Less", "Less or Equal" };
					static const std::array depthCompareOpValues = { vk::CompareOp::eNever, vk::CompareOp::eAlways, vk::CompareOp::eEqual, vk::CompareOp::eNotEqual, vk::CompareOp::eGreater, vk::CompareOp::eGreaterOrEqual, vk::CompareOp::eLess, vk::CompareOp::eLessOrEqual };
					int depthCompareOpIndex = IndexOf(depthCompareOpValues.begin(), depthCompareOpValues.end(), this->meshDepthCompareOp);
					ImGui::Combo("Depth Compare Op", &depthCompareOpIndex, depthCompareOpNames, IM_ARRAYSIZE(depthCompareOpNames));
					rebuildNeeded |= CheckIfChanged(this->meshDepthCompareOp, depthCompareOpValues[depthCompareOpIndex]);

					if (rebuildNeeded)
						this->RebuildMeshPipeline();

					ImGui::SeparatorText("Camera Controls:");
					ImGui::Indent();
					ImGui::DragFloat3("Position", glm::value_ptr(this->m_camera.m_position));
					ImGui::DragFloat3("Front", glm::value_ptr(this->m_camera.m_front));
					ImGui::DragFloat3("Up", glm::value_ptr(this->m_camera.m_up));
					ImGui::Unindent();

					ImGui::SeparatorText("Light Controls:");
					ImGui::Indent();
					ImGui::DragFloat3("Light Position", glm::value_ptr(this->cameraConstants.lightPosition));
					ImGui::ColorEdit3("Light Color", glm::value_ptr(this->cameraConstants.lightColor));

					ImGui::DragFloat3("Light Position2", glm::value_ptr(this->cameraConstants.lightPosition2));
					ImGui::ColorEdit3("Light Color2", glm::value_ptr(this->cameraConstants.lightColor2));
					ImGui::Unindent();
				}
				ImGui::End();
			});
        /////////////////////////////////////////////////////////////////////////

        vk::CommandBuffer commands;
        vk::Framebuffer framebuffer;

        m_pImguiManager->ProcessUI();

        if (!m_VulkanContext.BeginFrame(commands, framebuffer))
            return;

        // Update Camera Buffer
        commands.updateBuffer(cameraConstantsBuffer, 0, sizeof(cameraConstants), &cameraConstants);

        vk::RenderPassBeginInfo rpbi{};
        rpbi.setRenderPass(m_VulkanContext.GetDisplayRenderPass());
        rpbi.setFramebuffer(framebuffer);
        rpbi.setClearValues(clearValues);
        rpbi.setRenderArea({ { 0, 0 }, m_VulkanContext.GetSwapchain().extent });
        commands.beginRenderPass(rpbi, vk::SubpassContents::eInline);

        commands.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, gridPipeline->GetLayout(), 0, { cameraUniform }, { });
        commands.bindPipeline(vk::PipelineBindPoint::eGraphics, gridPipeline->GetPipeline());
        commands.draw(6, 1, 0, 0);

        // Draw Objects in Render Queue
        while (!m_RenderQueue.empty())
        {

            const RenderObject& obj = m_RenderQueue.front();
            m_RenderQueue.pop();

            // Bind meshes
            if (obj.mesh)
            {
				DebugBoxPushConstants debugConstants;
				debugConstants.objectMatrix = obj.transform;

                PushConstants pushConstants;
                pushConstants.objectMatrix = obj.transform;
                pushConstants.objectColor = obj.mesh->color;
				pushConstants.useTexture.x = obj.texture->IsNotNull() ? 1 : 0;

				// Draw mesh
				commands.bindPipeline(vk::PipelineBindPoint::eGraphics, meshPipeline->GetPipeline());
				commands.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, meshPipeline->GetLayout(), 0, { cameraUniform, *obj.texture->GetDescriptorSet() }, {});
				commands.pushConstants(meshPipeline->GetLayout(), vk::ShaderStageFlagBits::eAllGraphics, 0, sizeof(pushConstants), &pushConstants);
				commands.bindVertexBuffers(0, { obj.mesh->vertexBuffer }, { 0 });
				commands.bindIndexBuffer(obj.mesh->indexBuffer, 0, vk::IndexType::eUint32);
				commands.drawIndexed(obj.mesh->indexCount, 1, 0, 0, 0);

				// Draw debug box
				commands.bindPipeline(vk::PipelineBindPoint::eGraphics, debugBoxPipeline->GetPipeline());
				commands.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, debugBoxPipeline->GetLayout(), 0, { cameraUniform }, {});
				commands.pushConstants(debugBoxPipeline->GetLayout(), vk::ShaderStageFlagBits::eAllGraphics, 0, sizeof(DebugBoxPushConstants), &debugConstants);
				commands.bindVertexBuffers(0, { debugBoxMesh->vertexBuffer }, { 0 });
				commands.bindIndexBuffer(debugBoxMesh->indexBuffer, 0, vk::IndexType::eUint32);
				commands.drawIndexed(debugBoxMesh->indexCount, 1, 0, 0, 0);

 
            }
        }

		m_pImguiManager->ImGuiRender(commands);
        commands.endRenderPass();
        m_VulkanContext.EndFrame();
    }


    void VulkanRenderer::RebuildMeshPipeline()
    {
        m_VulkanContext.GetDevice().waitIdle();
		auto texture = Texture(m_VulkanContext);

        if (meshPipeline)
            m_VulkanContext.DestroyPipeline(meshPipeline);

		meshPipeline = m_VulkanContext.CreatePipeline(
			{
				.vertexBindings =
				{
					vk::VertexInputBindingDescription()
						.setBinding(0)
						.setInputRate(vk::VertexInputRate::eVertex)
						.setStride(sizeof(MeshVertex)),
				},
				.vertexAttributes =
				{
					vk::VertexInputAttributeDescription()
						.setLocation(0)
						.setBinding(0)
						.setFormat(vk::Format::eR32G32B32Sfloat)
						.setOffset(offsetof(MeshVertex, MeshVertex::position)),
					vk::VertexInputAttributeDescription()
						.setLocation(1)
						.setBinding(0)
						.setFormat(vk::Format::eR32G32B32Sfloat)
						.setOffset(offsetof(MeshVertex, MeshVertex::normal)),
					vk::VertexInputAttributeDescription()
						.setLocation(2)
						.setBinding(0)
						.setFormat(vk::Format::eR32G32Sfloat)
						.setOffset(offsetof(MeshVertex, MeshVertex::textCoord)),
				},
				.shaderPushConstants =
				{
					vk::PushConstantRange()
						.setOffset(0)
						.setSize(sizeof(PushConstants))
						.setStageFlags(vk::ShaderStageFlagBits::eAllGraphics)
				},
				.shaderUniforms =
				{
					cameraUniform,
					*texture.GetDescriptorLayout(),
				},
				.shaderStages =
				{
					vk::PipelineShaderStageCreateInfo()
						.setStage(vk::ShaderStageFlagBits::eVertex)
						.setPName("main")
						.setModule(m_shaderLibrary.GetModule("mesh.vert")),
					vk::PipelineShaderStageCreateInfo()
						.setStage(vk::ShaderStageFlagBits::eFragment)
						.setPName("main")
						.setModule(m_shaderLibrary.GetModule("mesh.frag"))
				},
                  .polygonMode = meshPolygonMode,
                  .cullMode = meshCullMode,
                  .frontFace = meshFrontFace,
                  .depthTestEnable = true,
                  .depthCompareOp = meshDepthCompareOp,
                  .depthWriteEnable = true,
                 .colorBlendStates =
                {
                    vk::PipelineColorBlendAttachmentState()
                        .setBlendEnable(true)
                        .setColorBlendOp(vk::BlendOp::eAdd)
                        .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
                        .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
                        .setAlphaBlendOp(vk::BlendOp::eAdd)
                        .setSrcAlphaBlendFactor(vk::BlendFactor::eSrcAlpha)
                        .setDstAlphaBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
                        .setColorWriteMask(vk::ColorComponentFlagBits::eA | vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB)
                },

              });

    }

    void VulkanRenderer::Destroy()
    {
		debugBoxMesh->Destroy();

        vkDeviceWaitIdle(m_VulkanContext.GetDevice());


        m_VulkanContext.DestroyUniform(cameraUniform);
        m_VulkanContext.DestroyBuffer(cameraConstantsBuffer);

        m_VulkanContext.DestroyPipeline(gridPipeline);

        m_VulkanContext.DestroyPipeline(meshPipeline);

        m_shaderLibrary.Shutdown();
        m_VulkanContext.Shutdown();
    }
}
