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
        SP_CORE_TRACE("Initializing Application");

        //m_Window = std::unique_ptr<Window>(Window::Create());

        GlobalEventDispatcher::Get().registerListener<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        GlobalEventDispatcher::Get().registerListener<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
        SP_CORE_TRACE("Application initializatized successfully!");
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
        Camera camera{};

        for (Layer* layer : m_LayerStack) {
            layer->OnInit(camera);
        }

        auto previousTime = std::chrono::high_resolution_clock::now();

        // End Initialize

        while (!m_GlobalLoader.getWindow().ShouldClose()) {
            // Start Update
            m_GlobalLoader.getWindow().OnUpdate();
            GlobalEventDispatcher::Get().updateEvents();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
            previousTime = currentTime;

            float aspect = m_GlobalLoader.getRenderer().getAspectRatio();
            camera.setOrthographicProjection(-aspect, aspect, -1.0f, 1.0f);

            while (frameTime > 0.0f) {
                float deltaTime = std::max(frameTime, 1.0f / 120.0f);

                for (Layer* layer : m_LayerStack) {
                    layer->OnUpdate(deltaTime, camera);
                }

                frameTime -= deltaTime;
            }
            // End Update

            // Start Rendering
            if (auto m_CommandBuffer = m_GlobalLoader.getRenderer().beginFrame()) {
                int frameIndex = m_GlobalLoader.getRenderer().getFrameIndex();

                FrameInfo frameInfo{ frameIndex, frameTime, m_CommandBuffer, camera, m_GlobalLoader.getGlobalDescriptorSets()[frameIndex], m_GlobalLoader.getGlobalSetLayout(), m_GlobalLoader.getGlobalDescriptorPool(), m_GlobalLoader.getGlobalUniformBuffers() };

                // Updating buffers
                GlobalUniformBuffer ubo{};
                ubo.projectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix();
                m_GlobalLoader.getGlobalUniformBuffers()[frameIndex]->writeToBuffer(&ubo);

                // Rendering frame
                m_GlobalLoader.getRenderer().beginSwapChainRenderPass(m_CommandBuffer);

                for (Layer* layer : m_LayerStack) {
                    layer->OnRender(frameInfo);
                }

                m_GlobalLoader.getRenderer().endSwapChainRenderPass(m_CommandBuffer);
                m_GlobalLoader.getRenderer().endFrame(frameInfo);
            }
            // End Rendering

            Input::Clear();
		}

        vkDeviceWaitIdle(m_GlobalLoader.getDevice().device());

	}
}