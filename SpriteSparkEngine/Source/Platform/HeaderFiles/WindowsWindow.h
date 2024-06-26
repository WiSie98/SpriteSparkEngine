#pragma once

#include "SparkCore/HeaderFiles/Window.h"

#include "SparkEvents/GlobalEventDispatcher.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace SpriteSpark {

	class WindowsWindow : public Window {

	public:

		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:

		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_Window;

		struct WindowData {

			std::string Titel;
			unsigned int Width = 1280, Height = 720;
			bool VSync = true;

			EventCallbackFn EventCallback;

		};

		WindowData m_Data;

		static void GLFWErrorCallback(int error, const char* description);
		static void GLFWWindowSizeCallback(GLFWwindow* window, int width, int height);
		static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	};

}