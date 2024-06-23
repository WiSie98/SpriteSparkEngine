#include "Sparkpch.h"

#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "SparkCore/HeaderFiles/Application.h"

#include "SparkEvents/Event.h"
#include "SparkEvents/ApplicationEvent.h"
#include "SparkEvents/KeyEvent.h"
#include "SparkEvents/MouseEvent.h"

namespace SpriteSpark {

	Application::Application() { }

	Application::~Application() { }

	void Application::Run() {

		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		GLFWwindow* window = glfwCreateWindow(800, 600, "SpriteSpark Game", nullptr, nullptr);

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		SP_CORE_DEBUG(extensionCount, " extensions supported");

		glm::mat4 matrix;
		glm::vec4 vec;
		auto test = matrix * vec;

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

		while (!glfwWindowShouldClose(window)) {

			glfwPollEvents();

			// Puffern von Events
			dispatcher.bufferEvent(std::make_shared<WindowResizeEvent>(1280, 720));
			dispatcher.bufferEvent(std::make_shared<KeyPressedEvent>(Key::B, false));
			dispatcher.bufferEvent(std::make_shared<MouseButtonPressedEvent>(Mouse::Button0));

			// Späteres Verarbeiten der gepufferten Events
			dispatcher.updateEvents();
		}

		glfwDestroyWindow(window);
		glfwTerminate();

	}

}