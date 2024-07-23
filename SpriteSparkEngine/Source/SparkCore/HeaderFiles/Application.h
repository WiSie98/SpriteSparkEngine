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
#include "Platform/Vulkan/HeaderFiles/VulkanBuffer.h"
#include "Platform/Vulkan/HeaderFiles/VulkanDescriptors.h"

namespace SpriteSpark {

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

	struct Rect {
		float x, y, width, height;

		Rect(float x, float y, float width, float height)
			: x(x), y(y), width(width), height(height) {}

		void getUVs(float& u_min, float& v_min, float& u_max, float& v_max, int textureSize) {
			u_min = x / textureSize;
			v_min = y / textureSize;
			u_max = (x + width) / textureSize;
			v_max = (y + height) / textureSize;
		}
	};

	class Application {

	public:

		Application();
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return m_Window; }

	private:

		WindowsWindow m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		VulkanDevice m_Device{ m_Window };
		Renderer m_Renderer{ m_Window, m_Device };

		std::unique_ptr<VulkanDescriptorPool> m_GlobalDescriptorPool{};
		std::vector<GameObject> m_GameObjects;

		static Application* s_Instance;

		void OnEvent(const Event& e);
		bool OnWindowResize(const WindowResizeEvent& e);
		bool OnWindowClose(const WindowCloseEvent& e);

		void loadGameObjects(float r, float g, float b, float a);

	};

	// Should be defined in Client
    Application* CreateApplication();

}