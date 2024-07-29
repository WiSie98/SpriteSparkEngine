#pragma once

#include "Sparkpch.h"

#include "SparkCore/HeaderFiles/GlobalLoader.h"

namespace SpriteSpark {

    GlobalLoader::GlobalLoader() {
        SP_CORE_TRACE("Initializing Vulkan window");
        m_GlobalDescriptorPool = VulkanDescriptorPool::Builder(m_Device)
            .setMaxSets(1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000)
            .build();

        createGlobalDescriptorSets();
        SP_CORE_TRACE("Vulkan window initializatized successfully!");

    }

    GlobalLoader::~GlobalLoader() {
        SP_CORE_INFO("Vulkan window closed successfully!");
    }

    void GlobalLoader::createGlobalDescriptorSets() {
        m_GlobalUniformBuffers.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < m_GlobalUniformBuffers.size(); i++) {
            m_GlobalUniformBuffers[i] = std::make_shared<VulkanBuffer>(
                m_Device,
                sizeof(GlobalUniformBuffer),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );
            VkResult result = m_GlobalUniformBuffers[i]->map();
            if (result != VK_SUCCESS) {
                throw std::runtime_error("failed to map global uniform buffer!");
            }
        }

        m_GlobalSetLayout = VulkanDescriptorSetLayout::Builder(m_Device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
            .build();

        m_GlobalDescriptorPool = VulkanDescriptorPool::Builder(m_Device)
            .setMaxSets(2000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2000)
            .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2000)
            .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
            .build();

        m_GlobalDescriptorSets.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < m_GlobalDescriptorSets.size(); i++) {
            auto bufferInfo = m_GlobalUniformBuffers[i]->descriptorInfo();
            VulkanDescriptorWriter writer(*m_GlobalSetLayout, *m_GlobalDescriptorPool);
            writer.writeBuffer(0, &bufferInfo);

            if (!writer.build(m_GlobalDescriptorSets[i])) {
                throw std::runtime_error("failed to build descriptor set!");
            }
        }
    }

	std::unique_ptr<VulkanTexture> GlobalLoader::LoadTexture(const std::string& texturePath) {
        try {
            auto texture = std::make_unique<SpriteSpark::VulkanTexture>(m_Device, texturePath);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.sampler = texture->getSampler();
            imageInfo.imageView = texture->getImageView();
            imageInfo.imageLayout = texture->getImageLayout();

            for (int i = 0; i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
                VulkanDescriptorWriter writer(*m_GlobalSetLayout, *m_GlobalDescriptorPool);
                writer.writeImage(1, &imageInfo);

                writer.overwrite(m_GlobalDescriptorSets[i]);
            }
            SP_CORE_INFO("texture loaded successfully from: ", texturePath);
            return texture;
        }
        catch (const std::exception& e) {
            SP_CORE_ERROR("failed to load texture from ", texturePath, " ", e.what());
            throw;
        }
	}

}