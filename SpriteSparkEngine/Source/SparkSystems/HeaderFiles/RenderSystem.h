#pragma once

#include "SparkCore/HeaderFiles/Core.h"
#include "SparkCore/HeaderFiles/Camera.h"
#include "SparkCore/HeaderFiles/GlobalLoader.h"
#include "SparkCore/HeaderFiles/FrameInfo.h"

#include "SparkECS/HeaderFiles/EntitySystems.h"
#include "SparkObjects/HeaderFiles/GameObject.h"

#include "Platform/Vulkan/HeaderFiles/VulkanDevice.h"
#include "Platform/Vulkan/HeaderFiles/VulkanPipeline.h"

namespace SpriteSpark {

	class RenderSystem : public System<RenderSystem> {

	public:

		RenderSystem();
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void renderGameObjects(FrameInfo& frameInfo, std::vector<GameObject>& gameObjects);

	private:

		VulkanDevice& m_Device;
		std::unique_ptr<VulkanPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;

		void createPipelineLayout(VkDescriptorSetLayout globalDescriptorSetLayout);
		void createPipeline(VkRenderPass renderPass);

	};

}