#pragma once

#include "SparkCore/HeaderFiles/Camera.h"

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
	};

}