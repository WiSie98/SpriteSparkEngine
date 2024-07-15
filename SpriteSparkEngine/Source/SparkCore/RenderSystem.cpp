#include "Sparkpch.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "SparkCore/HeaderFiles/RenderSystem.h"

namespace SpriteSpark {

    struct SimplePushConstantData {
        glm::mat2 transform{ 1.f };
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    RenderSystem::RenderSystem(VulkanDevice& device, VkRenderPass renderPass) : m_Device(device) {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    RenderSystem::~RenderSystem() {
        vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
    }

    void RenderSystem::createPipelineLayout() {

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void RenderSystem::createPipeline(VkRenderPass renderPass) {
        assert(m_PipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        VulkanPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = m_PipelineLayout;
        m_Pipeline = std::make_unique<VulkanPipeline>(m_Device, "Shaders/SimpleShader.vert.spv", "Shaders/SimpleShader.frag.spv", pipelineConfig);
    }

    void RenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects) {
        m_Pipeline->bind(commandBuffer);

        for (auto& obj : gameObjects) {
            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.0005f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();

            vkCmdPushConstants(commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }

}