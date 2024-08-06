#pragma once

#include "SparkCore/HeaderFiles/Core.h"
#include "SparkCore/HeaderFiles/Window.h"
#include "SparkCore/HeaderFiles/FrameInfo.h"
#include "Platform/Window/HeaderFiles/WindowsWindow.h"

#include "Platform/Vulkan/HeaderFiles/VulkanDevice.h"
#include "Platform/Vulkan/HeaderFiles/VulkanSwapChain.h"
#include "Platform/Vulkan/HeaderFiles/VulkanModel.h"

namespace SpriteSpark {

	class Renderer {

	public:

		Renderer(WindowsWindow& window, VulkanDevice& device);
		virtual ~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return m_SwapChain->getRenderPass(); }
		float getAspectRatio() const { return m_SwapChain->extentAspectRatio(); }
		float getWindowWidth() const { return static_cast<float>(m_SwapChain->getSwapChainExtent().width); }
		float getWindowHeight() const { return static_cast<float>(m_SwapChain->getSwapChainExtent().height); }
		bool isFrameInProgress() const { return m_IsFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(m_IsFrameStarted && "Cannot get command buffer when frame not in progress!");
			return m_CommandBuffers[m_CurrentFrameIndex];
		}

		int getFrameIndex() const {
			assert(m_IsFrameStarted && "Cannot get frame index when frame not in progress!");
			return m_CurrentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame(FrameInfo& frameInfo);
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:

		WindowsWindow& m_Window;
		VulkanDevice& m_Device;
		std::unique_ptr<VulkanSwapChain> m_SwapChain;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		uint32_t m_CurrentImageIndex;
		int m_CurrentFrameIndex = 0;
		bool m_IsFrameStarted{false};

		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();


	};

}