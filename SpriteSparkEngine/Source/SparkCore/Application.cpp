#include "Sparkpch.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "SparkCore/HeaderFiles/Application.h"

namespace SpriteSpark {

	Application::Application() {

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        // Event-Listener registrieren mit Testfunktionen
        auto& dispatcher = GlobalEventDispatcher::Get();
        dispatcher.registerListener<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.registerListener<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

        dispatcher.registerListener<KeyPressedEvent>([](const KeyPressedEvent& e) {
            SP_CORE_INFO("Key pressed: ", e.GetKeyCode());
        });

        dispatcher.registerListener<MouseMovedEvent>([](const MouseMovedEvent& e) {
            SP_CORE_INFO("Mouse moved to: ", e.GetX(), ", ", e.GetY());
        });
	}

	Application::~Application() { }

    void Application::OnEvent(Event& e) {

    }

    bool Application::OnWindowResize(WindowResizeEvent& e) {
        SP_CORE_INFO("Window resized to ", e.GetWidth(), "x", e.GetHeight());
        return false;
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_Running = false;
        SP_CORE_INFO("Window Closed");
        return true;
    }

	void Application::Run() {

		while (m_Running) {
			m_Window->OnUpdate();
            SpriteSpark::GlobalEventDispatcher::Get().updateEvents();
		}

	}

}