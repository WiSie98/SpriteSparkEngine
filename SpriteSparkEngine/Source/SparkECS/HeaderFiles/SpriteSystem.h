#pragma once

#include "SparkCore/HeaderFiles/Core.h"
#include "SparkCore/HeaderFiles/Camera.h"
#include "SparkCore/HeaderFiles/GlobalLoader.h"
#include "SparkCore/HeaderFiles/FrameInfo.h"

#include "SparkECS/HeaderFiles/EntityComponents.h"
#include "SparkECS/HeaderFiles/EntityManager.h"
#include "SparkECS/HeaderFiles/EntitySystems.h"

#include "Platform/Vulkan/HeaderFiles/VulkanDevice.h"
#include "Platform/Vulkan/HeaderFiles/VulkanPipeline.h"

namespace SpriteSpark {

	class SpriteSystem : public System<SpriteSystem> {

	public:

		SpriteSystem();
		~SpriteSystem();

		SpriteSystem(const SpriteSystem&) = delete;
		SpriteSystem& operator=(const SpriteSystem&) = delete;

		void render(FrameInfo& frameInfo, EntityManager& entities);

	private:

		VulkanDevice& m_Device;
		std::unique_ptr<VulkanPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;

		void createPipelineLayout(VkDescriptorSetLayout globalDescriptorSetLayout);
		void createPipeline(VkRenderPass renderPass);

	};
}