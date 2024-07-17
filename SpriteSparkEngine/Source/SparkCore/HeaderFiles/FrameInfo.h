#pragma once

#include "SparkCore/HeaderFiles/Camera.h"

#include "vulkan/vulkan.h"

namespace SpriteSpark {

	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		Camera camera;
		VkDescriptorSet globalDescriptorSet;
	};

}