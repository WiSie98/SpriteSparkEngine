#pragma once

#include "SparkCore/HeaderFiles/Camera.h"

#include "Platform/Vulkan/HeaderFiles/VulkanDescriptors.h"
#include "Platform/Vulkan/HeaderFiles/VulkanBuffer.h"

#include "vulkan/vulkan.h"

namespace SpriteSpark {

	struct GlobalUniformBuffer {
		glm::mat4 projectionMatrix{ 1.0f };
		glm::mat2 transform{ 1.0f };
	};

    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        Camera camera;
        VkDescriptorSet globalDescriptorSet;
        std::shared_ptr<VulkanDescriptorSetLayout> globalDescriptorSetLayout;
        std::shared_ptr<VulkanDescriptorPool> globalDescriptorPool;
        std::vector<std::shared_ptr<VulkanBuffer>> globalUniformBuffers;
        std::vector<std::vector<VkDescriptorSet>> descriptorSetsToFree;

        ~FrameInfo() {
            for (auto& descriptorSetList : descriptorSetsToFree) {
                if (globalDescriptorPool) {
                    globalDescriptorPool->freeDescriptors(descriptorSetList);
                }
            }
            descriptorSetsToFree.clear();

            for (auto& buffer : globalUniformBuffers) {
                buffer.reset();
            }
            globalUniformBuffers.clear();

            globalDescriptorSetLayout.reset();
            globalDescriptorPool.reset();
        }
    };
}