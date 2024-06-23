#include "Sparkpch.h"

#include "SparkCore/HeaderFiles/Application.h"

#include "SparkEvents/Event.h"
#include "SparkEvents/ApplicationEvent.h"
#include "SparkEvents/KeyEvent.h"
#include "SparkEvents/MouseEvent.h"

namespace SpriteSpark {

	Application::Application() { }

	Application::~Application() { }

	void Application::Run() {

		SpriteSpark::EventDispatcher dispatcher;
		SpriteSpark::WindowResizeEvent window_event(1280, 720);

		// Registrierung eines Listeners für WindowResizeEvent
		dispatcher.registerListener<WindowResizeEvent>([](const WindowResizeEvent& event) {
			SP_CORE_TRACE(event, " in Application.cpp");
		});

		dispatcher.registerListener<KeyPressedEvent>([](const KeyPressedEvent& event) {
			SP_CORE_TRACE(event, " in Application.cpp");
		});

		dispatcher.registerListener<MouseButtonPressedEvent>([](const MouseButtonPressedEvent& event) {
			SP_CORE_TRACE(event, " in Application.cpp");
		});

		while (true) {

			// Puffern von Events
			dispatcher.bufferEvent(std::make_shared<WindowResizeEvent>(1280, 720));
			dispatcher.bufferEvent(std::make_shared<KeyPressedEvent>(Key::B, false));
			dispatcher.bufferEvent(std::make_shared<MouseButtonPressedEvent>(Mouse::Button0));

			// Späteres Verarbeiten der gepufferten Events
			dispatcher.updateEvents();
			std::cin.get();
		}
	}

}