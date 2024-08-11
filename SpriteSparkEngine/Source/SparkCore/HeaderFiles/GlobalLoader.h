#pragma once

#include "Platform/Window/HeaderFiles/WindowsWindow.h"

#include "Platform/Vulkan/HeaderFiles/VulkanDevice.h"
#include "Platform/Vulkan/HeaderFiles/VulkanModel.h"
#include "Platform/Vulkan/HeaderFiles/VulkanBuffer.h"
#include "Platform/Vulkan/HeaderFiles/VulkanDescriptors.h"
#include "Platform/Vulkan/HeaderFiles/VulkanTexture.h"
#include "Platform/Vulkan/HeaderFiles/VulkanSwapChain.h"

#include "SparkCore/HeaderFiles/Renderer.h"
#include "SparkCore/HeaderFiles/Core.h"
#include "SparkCore/HeaderFiles/FrameInfo.h"

#include "SparkECS/HeaderFiles/EntityManager.h"

namespace SpriteSpark {

	class GlobalLoader {

	public:

		GlobalLoader();
		~GlobalLoader();

		inline static GlobalLoader& Get() {
			static std::unique_ptr <GlobalLoader> globalLoader = std::make_unique<GlobalLoader>();
			return *globalLoader;
		}

		WindowsWindow& getWindow() { return m_Window; }
		VulkanDevice& getDevice() { return m_Device; }
		Renderer& getRenderer() { return m_Renderer; }

		std::shared_ptr<VulkanDescriptorPool>& getGlobalDescriptorPool() { return m_GlobalDescriptorPool; }
		std::vector<std::shared_ptr<VulkanBuffer>>& getGlobalUniformBuffers() { return m_GlobalUniformBuffers; }
		std::vector<VkDescriptorSet>& getGlobalDescriptorSets() { return m_GlobalDescriptorSets; }
		std::shared_ptr<VulkanDescriptorSetLayout>& getGlobalSetLayout() { return m_GlobalSetLayout; }

		static std::unique_ptr<VulkanTexture> LoadTexture(const std::string& texturePath);
		static void UnloadTexture(std::unique_ptr<VulkanTexture>& texture);

		static void LoadSprites(EntityManager& entityManager, int layer, std::unique_ptr<VulkanTexture>& texture, const std::string& tilesetDescriptionFilepath, const std::string& tilesetMapFilepath);
		static void LoadCollider(EntityManager& entityManager, int layers, const std::string& tilesetDescriptionFilepath, const std::string& tilesetMapFilepath);

	private:

		void createGlobalDescriptorSets();

		WindowsWindow m_Window;
		VulkanDevice m_Device{ m_Window };
		Renderer m_Renderer{ m_Window, m_Device };

		std::shared_ptr<VulkanDescriptorPool> m_GlobalDescriptorPool{};
		std::vector<std::shared_ptr<VulkanBuffer>> m_GlobalUniformBuffers;
		std::vector<VkDescriptorSet> m_GlobalDescriptorSets;
		std::shared_ptr<VulkanDescriptorSetLayout> m_GlobalSetLayout;

	};

}