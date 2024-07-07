#pragma once

#include "SparkCore/HeaderFiles/Core.h"
#include "SparkCore/HeaderFiles/Window.h"
#include "SparkCore/HeaderFiles/LayerStack.h"
#include "SparkEvents/GlobalEventDispatcher.h"

namespace SpriteSpark {

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

	class Application {

	public:

		Application();
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		void OnEvent(const Event& e);
		bool OnWindowResize(const WindowResizeEvent& e);
		bool OnWindowClose(const WindowCloseEvent& e);

	};

	// Should be defined in Client
    Application* CreateApplication();

}