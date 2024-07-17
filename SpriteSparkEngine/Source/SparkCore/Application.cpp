#include "Sparkpch.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "SparkCore/HeaderFiles/Application.h"
#include "SparkCore/HeaderFiles/GlobalThreadPool.h"
#include "SparkCore/HeaderFiles/Input.h"
#include "SparkSystems/HeaderFiles/RenderSystem.h"
#include "SparkCore/HeaderFiles/Camera.h"

namespace SpriteSpark {

	Application::Application() {

        //m_Window = std::unique_ptr<Window>(Window::Create());
        m_GlobalDescriptorPool = VulkanDescriptorPool::Builder(m_Device)
            .setMaxSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
        
        loadGameObjects();

        EventDispatcher& dispatcher = GlobalEventDispatcher::Get();
        Input::Initialize(dispatcher);

        dispatcher.registerListener<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.registerListener<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

	}

	Application::~Application() {
        SP_CORE_INFO("Application closed successfully!");
    }

    void Application::OnEvent(const Event& e) {
        SP_CORE_ERROR("Test");
    }

    bool Application::OnWindowResize(const WindowResizeEvent& e) {
        //SP_CORE_INFO("Window resized to ", e.GetWidth(), "x", e.GetHeight());
        return false;
    }

    bool Application::OnWindowClose(const WindowCloseEvent& e) {
        m_Running = false;
        SP_CORE_INFO("Window Closed");
        return true;
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay) {
        m_LayerStack.PushOverlay(overlay);
    }

	void Application::Run() {
        // Start Initialize

        std::vector<std::unique_ptr<VulkanBuffer>> globalUniformBuffers(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalUniformBuffers.size(); i++) {
            globalUniformBuffers[i] = std::make_unique<VulkanBuffer>(
                m_Device,
                sizeof(GlobalUniformBuffer),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            );
            globalUniformBuffers[i]->map();
        }

        auto globalSetLayout = VulkanDescriptorSetLayout::Builder(m_Device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = globalUniformBuffers[i]->descriptorInfo();
            VulkanDescriptorWriter(*globalSetLayout, *m_GlobalDescriptorPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        RenderSystem renderSystem{ m_Device, m_Renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
        Camera camera{};

        auto previousTime = std::chrono::high_resolution_clock::now();
        float cameraVelocity = 1.00f;
        float spriteVelocity = 0.05f;

        // End Initialize

        while (!m_Window.ShouldClose()) {
            // Start Update

            m_Window.OnUpdate();
            GlobalEventDispatcher::Get().updateEvents();

            auto currentTime = std::chrono::high_resolution_clock::now();
            double frameTime = std::chrono::duration<double, std::chrono::seconds::period>(currentTime - previousTime).count();
            previousTime = currentTime;

            float aspect = m_Renderer.getAspectRatio();
            camera.setOrthographicProjection(-aspect, aspect, -1.6f, 1.6f);

            while (frameTime > 0.0f) {
                double deltaTime = std::max(frameTime, 1.0/120.0);
                if (m_GameObjects[0].transform2d.translation.x > 2) {
                    spriteVelocity = -0.05f;
                } else if (m_GameObjects[0].transform2d.translation.x < -2) {
                    spriteVelocity = 0.05f;
                }

                m_GameObjects[0].transform2d.translation.x += spriteVelocity * deltaTime;
                m_GameObjects[0].transform2d.rotation += 0.05f * deltaTime;

                for (Layer* layer : m_LayerStack) {
                    layer->OnUpdate(deltaTime);
                }

                // Camera test
                if (Input::IsKeyPressed(Key::W)) {
                    camera.setPosition({camera.getPosition().x, camera.getPosition().y - cameraVelocity * deltaTime, 0.0f });
                } else if (Input::IsKeyPressed(Key::S)) {
                    camera.setPosition({ camera.getPosition().x, camera.getPosition().y + cameraVelocity * deltaTime, 0.0f });
                } else if (Input::IsKeyPressed(Key::A)) {
                    camera.setPosition({ camera.getPosition().x - cameraVelocity * deltaTime, camera.getPosition().y, 0.0f });
                } else if (Input::IsKeyPressed(Key::D)) {
                    camera.setPosition({ camera.getPosition().x + cameraVelocity * deltaTime, camera.getPosition().y, 0.0f });
                }

                if (Input::IsKeyPressed(Key::Q)) {
                    camera.setZoom(camera.getZoom() + cameraVelocity * deltaTime);
                } else if (Input::IsKeyPressed(Key::E)) {
                    camera.setZoom(camera.getZoom() - cameraVelocity * deltaTime);
                }

                frameTime -= deltaTime;
            }
            // End Update

            // Start Rendering
            if (auto m_CommandBuffer = m_Renderer.beginFrame()) {
                int frameIndex = m_Renderer.getFrameIndex();
                FrameInfo frameInfo{ frameIndex, frameTime, m_CommandBuffer, camera, globalDescriptorSets[frameIndex] };

                // Updating buffers
                GlobalUniformBuffer ubo{};
                ubo.projectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix();
                globalUniformBuffers[frameIndex]->writeToBuffer(&ubo);

                // Rendering frame
                m_Renderer.beginSwapChainRenderPass(m_CommandBuffer);
                renderSystem.renderGameObjects(frameInfo, m_GameObjects);
                m_Renderer.endSwapChainRenderPass(m_CommandBuffer);
                m_Renderer.endFrame();
            }
            // End Rendering

            Input::Clear();
		}

        vkDeviceWaitIdle(m_Device.device());

	}

    void Application::loadGameObjects() {
        VulkanModel::Data modelData{};

        //sierpinski(modelData.vertices, 10, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });
        modelData.vertices.push_back({ { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } });
        modelData.vertices.push_back({ { 0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f } });
        modelData.vertices.push_back({ { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } });
        modelData.vertices.push_back({ { -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f } });

        modelData.indices = {0, 1, 2, 1, 2, 3};

        auto m_Model = std::make_shared<VulkanModel>(m_Device, modelData);
        auto gameObj = GameObject::createGameObject();
        gameObj.model = m_Model;
        gameObj.color = { 0.1f, 0.1f, 0.8f };
        //gameObj.transform2d.translation.x = 1.0f;
        //gameObj.transform2d.scale = { 2.0f, 0.5f };
        //gameObj.transform2d.rotation = 0.25f * glm::two_pi<float>();

        m_GameObjects.push_back(std::move(gameObj));
    }

    void Application::sierpinski(std::vector<VulkanModel::Vertex>& vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top) {
        if (depth <= 0) {
            vertices.push_back({ top, { 0.0f, 0.0f, 1.0f } });
            vertices.push_back({ right, { 0.0f, 1.0f, 0.0f } });
            vertices.push_back({ left, { 1.0f, 0.0f, 0.0f } });
        } else {
            auto leftTop = 0.5f * (left + top);
            auto rightTop = 0.5f * (right + top);
            auto leftRight = 0.5f * (left + right);
            sierpinski(vertices, depth - 1, left, leftRight, leftTop);
            sierpinski(vertices, depth - 1, leftRight, right, rightTop);
            sierpinski(vertices, depth - 1, leftTop, rightTop, top);
        }
    }

}