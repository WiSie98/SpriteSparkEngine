#pragma once

#include "SparkCore/HeaderFiles/Core.h"
#include "SparkCore/HeaderFiles/Window.h"
#include "Platform/Window/HeaderFiles/WindowsWindow.h"
#include "SparkCore/HeaderFiles/LayerStack.h"
#include "SparkEvents/GlobalEventDispatcher.h"
#include "SparkObjects/HeaderFiles/GameObject.h"
#include "SparkCore/HeaderFiles/Renderer.h"
#include "SparkCore/HeaderFiles/GlobalLoader.h"

#include "Platform/Vulkan/HeaderFiles/VulkanDevice.h"
#include "Platform/Vulkan/HeaderFiles/VulkanModel.h"
#include "Platform/Vulkan/HeaderFiles/VulkanBuffer.h"
#include "Platform/Vulkan/HeaderFiles/VulkanDescriptors.h"
#include "Platform/Vulkan/HeaderFiles/VulkanTexture.h"

namespace SpriteSpark {

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

	class Application {

	public:

		Application();
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return m_GlobalLoader.getWindow(); }
		VulkanDevice& getDevice() { return m_GlobalLoader.getDevice(); };

	private:

		bool m_Running = true;
		LayerStack m_LayerStack;

		GlobalLoader& m_GlobalLoader = GlobalLoader::Get();

		static Application* s_Instance;

		void OnEvent(const Event& e);
		bool OnWindowResize(const WindowResizeEvent& e);
		bool OnWindowClose(const WindowCloseEvent& e);

	};

	// Should be defined in Client
    Application* CreateApplication();

}