#pragma once

#include "Sparkpch.h"

#include "SparkCore/HeaderFiles/GlobalLoader.h"

#include "SparkECS/HeaderFiles/EntityComponents.h"

#include "External/json.h"

namespace SpriteSpark {

    GlobalLoader::GlobalLoader() {
        SP_CORE_TRACE("Initializing Vulkan window");
        m_GlobalDescriptorPool = VulkanDescriptorPool::Builder(m_Device)
            .setMaxSets(10000)
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
            .setMaxSets(20000)
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
            auto texture = std::make_unique<SpriteSpark::VulkanTexture>(GlobalLoader::Get().getDevice(), texturePath);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.sampler = texture->getSampler();
            imageInfo.imageView = texture->getImageView();
            imageInfo.imageLayout = texture->getImageLayout();

            for (int i = 0; i < VulkanSwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
                VulkanDescriptorWriter writer(*GlobalLoader::Get().getGlobalSetLayout(), *GlobalLoader::Get().getGlobalDescriptorPool());
                writer.writeImage(1, &imageInfo);

                writer.overwrite(GlobalLoader::Get().getGlobalDescriptorSets()[i]);
            }
            SP_CORE_INFO("texture loaded successfully from: ", texturePath);
            return texture;
        }
        catch (const std::exception& e) {
            SP_CORE_ERROR("failed to load texture from ", texturePath, " ", e.what());
            throw;
        }
	}

    void GlobalLoader::UnloadTexture(std::unique_ptr<VulkanTexture>& texture) {
        texture.reset();
    }

    void GlobalLoader::LoadSprites(EntityManager& entityManager, std::unique_ptr<VulkanTexture>& texture, const std::string& tilesetDescriptionFilepath, const std::string& tilesetMapFilepath) {
        //Loading the tileset description file.
        std::ifstream tileset_description_file(tilesetDescriptionFilepath);
        nlohmann::json tileset_description = nlohmann::json::parse(tileset_description_file);
        tileset_description_file.close();

        //Loading the level file.
        std::ifstream level_map_file(tilesetMapFilepath);
        nlohmann::json level_map = nlohmann::json::parse(level_map_file);
        level_map_file.close();

        glm::vec2 vec = { 0, 0 };
        Rect rec = { 0, 0, level_map["tilewidth"], level_map["tileheight"] };

        for (auto const& layer : level_map["layers"]) {
            if (layer["type"] == "tilelayer" && layer["visible"]) {
                for (auto const& tileId : layer["data"]) {
                    if (layer["id"] < 3) {
                        if (tileId != 0) {
                            rec.x = static_cast<float>(((static_cast<int>(tileId) - 1) % static_cast<int>(tileset_description["columns"]))) * static_cast<float>(level_map["tilewidth"]);
                            rec.y = static_cast<float>(floor(static_cast<float>(tileId) / static_cast<float>(tileset_description["columns"]))) * static_cast<float>(level_map["tilewidth"]);

                            if (static_cast<int>(tileId) % 20 == 0) {
                                rec.y -= 16;
                            }

                            Entity entity = entityManager.createEntity();

                            entityManager.addComponent(entity, Transform{ vec, {1.0f, 1.0f}, 0.0f });
                            entityManager.addComponent(entity, Sprite(texture, rec));
                        }
                    }
                    else {
                        return;
                    }

                    vec.x += static_cast<float>(level_map["tilewidth"]);
                    if (vec.x >= static_cast<float>(layer["width"]) * static_cast<float>(level_map["tilewidth"])) {
                        vec.x = 0;
                        vec.y += static_cast<float>(level_map["tileheight"]);
                    }
                    if (vec.y >= static_cast<float>(layer["height"]) * static_cast<float>(level_map["tileheight"])) {
                        vec.y = 0;
                    }
                }
            }

        }
    }

}