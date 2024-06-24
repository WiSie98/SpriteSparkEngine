#pragma once

#include "SparkCore/HeaderFiles/Core.h"
#include "SparkCore/HeaderFiles/Window.h"

#include "SparkEvents/GlobalEventDispatcher.h"

namespace SpriteSpark {

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

	class Application {

	public:

		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

	private:

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

	};

	// Should be defined in Client
    Application* CreateApplication();

}