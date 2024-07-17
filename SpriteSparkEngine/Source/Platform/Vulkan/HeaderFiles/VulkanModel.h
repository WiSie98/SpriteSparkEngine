#pragma once

#include "vulkan/vulkan.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "Platform/Vulkan/HeaderFiles/VulkanDevice.h"
#include "Platform/Vulkan/HeaderFiles/VulkanBuffer.h"

namespace SpriteSpark {

	class VulkanModel {

	public:

		struct Vertex {
			glm::vec2 position{};
			glm::vec3 color{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		struct Data {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};
		};

		VulkanModel(VulkanDevice& device, const Data& data);
		~VulkanModel();

		VulkanModel(const VulkanModel&) = delete;
		VulkanModel& operator=(const VulkanModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:

		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffer(const std::vector<uint32_t>& indices);

		VulkanDevice& m_Device;

		std::unique_ptr<VulkanBuffer> m_VertexBuffer;
		uint32_t m_VertexCount;

		bool m_HasIndexBuffer = false;
		std::unique_ptr<VulkanBuffer> m_IndexBuffer;
		uint32_t m_IndexCount;

	};

}