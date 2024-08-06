#pragma once

#include "Platform/Vulkan/HeaderFiles/VulkanDevice.h"
#include "vulkan/vulkan_core.h"

namespace SpriteSpark {

	class VulkanTexture {

	public:

		VulkanTexture() = default;
		VulkanTexture(VulkanDevice& device, const std::string& filepath);
		~VulkanTexture();

		VulkanTexture(const VulkanTexture&) = delete;
		VulkanTexture& operator=(const VulkanTexture&) = delete;
		VulkanTexture(VulkanTexture&&) = delete;
		VulkanTexture& operator=(VulkanTexture&&) = delete;

		VkSampler getSampler() const { return m_Sampler; }
		VkImageView getImageView() const { return m_ImageView; }
		VkImageLayout getImageLayout() const { return m_ImageLayout; }

		int getWidth() { return m_Width; };
		int getHeight() { return m_Height; };

	private:

		void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
		void generateMipmaps();

		int m_Width, m_Height, m_MipLevels;

		VulkanDevice& m_Device;
		VkImage m_Image;
		VkDeviceMemory m_ImageMemory;
		VkImageView m_ImageView;
		VkSampler m_Sampler;
		VkFormat m_ImageFormat;
		VkImageLayout m_ImageLayout;

	};

}