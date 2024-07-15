#pragma once

#include "SparkCore/HeaderFiles/Core.h"
#include "SparkObjects/HeaderFiles/GameObject.h"

#include "Platform/Vulkan/HeaderFiles/VulkanDevice.h"
#include "Platform/Vulkan/HeaderFiles/VulkanPipeline.h"

namespace SpriteSpark {

	class RenderSystem {

	public:

		RenderSystem(VulkanDevice& device, VkRenderPass renderPass);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects);

	private:

		VulkanDevice& m_Device;
		std::unique_ptr<VulkanPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;

		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

	};

}