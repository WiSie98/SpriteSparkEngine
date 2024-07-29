#include "Sparkpch.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "SparkCore/HeaderFiles/FrameInfo.h"
#include "SparkSystems/HeaderFiles/RenderSystem.h"

#include "Platform/Vulkan/HeaderFiles/VulkanDescriptors.h"

namespace SpriteSpark {

    struct SimplePushConstantData {
        glm::mat2 transform{ 1.0f };
        glm::vec2 offset;
        alignas(16) glm::vec4 color;
        glm::vec2 uv;
    };

    RenderSystem::RenderSystem() : m_Device(GlobalLoader::Get().getDevice()) {
        createPipelineLayout(GlobalLoader::Get().getGlobalSetLayout()->getDescriptorSetLayout());
        createPipeline(GlobalLoader::Get().getRenderer().getSwapChainRenderPass());
    }

    RenderSystem::~RenderSystem() {
        vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
    }

    void RenderSystem::createPipelineLayout(VkDescriptorSetLayout globalDescriptorSetLayout) {
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts = { globalDescriptorSetLayout, globalDescriptorSetLayout };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

    void RenderSystem::createPipeline(VkRenderPass renderPass) {
        assert(m_PipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");

        PipelineConfigInfo pipelineConfig{};
        VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
        VulkanPipeline::enableAlphaBlending(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = m_PipelineLayout;
        m_Pipeline = std::make_unique<VulkanPipeline>(m_Device, "Shaders/SimpleShader.vert.spv", "Shaders/SimpleShader.frag.spv", pipelineConfig);
    }

    void RenderSystem::renderGameObjects(FrameInfo& frameInfo, std::vector<GameObject>& gameObjects) {
        m_Pipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);

        std::vector<VkDescriptorSet> tempDescriptorSetsToFree;

        for (auto& obj : gameObjects) {
            SimplePushConstantData push{};
            push.transform = obj.transform2d.mat2();
            push.offset = obj.transform2d.translation;
            push.color = obj.color;

            vkCmdPushConstants(frameInfo.commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

            if (obj.texture) {
                VkDescriptorImageInfo imageInfo{};
                imageInfo.sampler = obj.texture->getSampler();
                imageInfo.imageView = obj.texture->getImageView();
                imageInfo.imageLayout = obj.texture->getImageLayout();

                VkDescriptorSet tempDescriptorSet;
                if (!frameInfo.globalDescriptorPool->allocateDescriptor(frameInfo.globalDescriptorSetLayout->getDescriptorSetLayout(), tempDescriptorSet)) {
                    throw std::runtime_error("failed to allocate descriptor sets!");
                }

                VkWriteDescriptorSet descriptorWrites[2] = {};

                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = (frameInfo.globalUniformBuffers)[frameInfo.frameIndex]->getBuffer();
                bufferInfo.offset = 0;
                bufferInfo.range = sizeof(GlobalUniformBuffer);

                descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[0].dstSet = tempDescriptorSet;
                descriptorWrites[0].dstBinding = 0;
                descriptorWrites[0].dstArrayElement = 0;
                descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[0].descriptorCount = 1;
                descriptorWrites[0].pBufferInfo = &bufferInfo;

                descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[1].dstSet = tempDescriptorSet;
                descriptorWrites[1].dstBinding = 1;
                descriptorWrites[1].dstArrayElement = 0;
                descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrites[1].descriptorCount = 1;
                descriptorWrites[1].pImageInfo = &imageInfo;

                vkUpdateDescriptorSets(m_Device.device(), 2, descriptorWrites, 0, nullptr);

                vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 1, 1, &tempDescriptorSet, 0, nullptr);

                tempDescriptorSetsToFree.push_back(tempDescriptorSet);
            }

            obj.model->bind(frameInfo.commandBuffer);
            obj.model->draw(frameInfo.commandBuffer);
        }

        frameInfo.descriptorSetsToFree.push_back(std::move(tempDescriptorSetsToFree));
    }

}