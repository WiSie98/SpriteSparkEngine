#include "Sparkpch.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "SparkCore/HeaderFiles/Application.h"
#include "SparkCore/HeaderFiles/GlobalThreadPool.h"
#include "SparkCore/HeaderFiles/Input.h"

namespace SpriteSpark {

	Application::Application() {

        m_Window = std::unique_ptr<Window>(Window::Create());

        // Event-Listener registrieren mit Testfunktionen
        EventDispatcher& dispatcher = GlobalEventDispatcher::Get();
        Input::Initialize(dispatcher);

        dispatcher.registerListener<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.registerListener<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

	}

	Application::~Application() { }

    void Application::OnEvent(const Event& e) {
        SP_CORE_ERROR("Test");
    }

    bool Application::OnWindowResize(const WindowResizeEvent& e) {
        SP_CORE_INFO("Window resized to ", e.GetWidth(), "x", e.GetHeight());
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

		while (m_Running) {

            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate();
            }

			m_Window->OnUpdate();
            Input::Clear();
            GlobalEventDispatcher::Get().updateEvents();
		}

	}

}