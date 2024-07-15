#pragma once

#include "SparkCore/HeaderFiles/Core.h"
#include "SparkCore/HeaderFiles/Window.h"
#include "Platform/Window/HeaderFiles/WindowsWindow.h"
#include "SparkCore/HeaderFiles/LayerStack.h"
#include "SparkEvents/GlobalEventDispatcher.h"
#include "SparkObjects/HeaderFiles/GameObject.h"
#include "SparkCore/HeaderFiles/Renderer.h"

#include "Platform/Vulkan/HeaderFiles/VulkanDevice.h"
#include "Platform/Vulkan/HeaderFiles/VulkanModel.h"

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

		WindowsWindow m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		VulkanDevice m_Device{ m_Window };
		Renderer m_Renderer{ m_Window, m_Device };
		std::vector<GameObject> m_GameObjects;

		void OnEvent(const Event& e);
		bool OnWindowResize(const WindowResizeEvent& e);
		bool OnWindowClose(const WindowCloseEvent& e);

		void loadGameObjects();

		void sierpinski(std::vector<VulkanModel::Vertex>& vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top);

	};

	// Should be defined in Client
    Application* CreateApplication();

}