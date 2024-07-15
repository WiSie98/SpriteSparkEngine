#include "Sparkpch.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "SparkCore/HeaderFiles/Application.h"
#include "SparkCore/HeaderFiles/GlobalThreadPool.h"
#include "SparkCore/HeaderFiles/Input.h"
#include "SparkCore/HeaderFiles/RenderSystem.h"

namespace SpriteSpark {

	Application::Application() {

        //m_Window = std::unique_ptr<Window>(Window::Create());

        loadGameObjects();

        EventDispatcher& dispatcher = GlobalEventDispatcher::Get();
        Input::Initialize(dispatcher);

        dispatcher.registerListener<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.registerListener<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

	}

	Application::~Application() {}

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
        RenderSystem renderSystem{ m_Device, m_Renderer.getSwapChainRenderPass() };

        while (!m_Window.ShouldClose()) {

            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate();
            }

			m_Window.OnUpdate();

            if (auto m_CommandBuffer = m_Renderer.beginFrame()) {
                m_Renderer.beginSwapChainRenderPass(m_CommandBuffer);
                renderSystem.renderGameObjects(m_CommandBuffer, m_GameObjects);
                m_Renderer.endSwapChainRenderPass(m_CommandBuffer);
                m_Renderer.endFrame();
            }

            Input::Clear();
            GlobalEventDispatcher::Get().updateEvents();
		}

        vkDeviceWaitIdle(m_Device.device());

	}

    void Application::loadGameObjects() {
        std::vector<VulkanModel::Vertex> vertices{};

        sierpinski(vertices, 7, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });

        auto m_Model = std::make_shared<VulkanModel>(m_Device, vertices);
        auto triangle = GameObject::createGameObject();
        triangle.model = m_Model;
        triangle.color = { 0.1f, 0.8f, 0.1f };
        triangle.transform2d.translation.x = 0.2f;
        triangle.transform2d.scale = { 2.0f, 0.5f };
        triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

        m_GameObjects.push_back(std::move(triangle));
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