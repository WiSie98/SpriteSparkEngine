#include "Sparkpch.h"

#include "Platform/Window/HeaderFiles/WindowsWindow.h"

namespace SpriteSpark {
	
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow() {
		WindowProps defaultProps;
		Init(defaultProps);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Titel = props.Titel;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		SP_CORE_TRACE("Creating window ", props.Titel, ", Resolution: ", props.Width, "x", props.Height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			SP_CORE_ASSERT(success, " Could not initialize GLFW!");

			s_GLFWInitialized = true;
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Titel.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetFramebufferSizeCallback(m_Window, GLFWWindowSizeCallback);

		/*
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		*/
		glfwSetWindowSizeCallback(m_Window, GLFWWindowSizeCallback);
		glfwSetKeyCallback(m_Window, GLFWKeyCallback);
		glfwSetMouseButtonCallback(m_Window, GLFWMouseButtonCallback);
		glfwSetCursorPosCallback(m_Window, GLFWCursorPosCallback);
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		//glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, m_Window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	bool WindowsWindow::ShouldClose() {
		return glfwWindowShouldClose(m_Window);
	}

	void WindowsWindow::setShouldClose(bool value) {
		glfwSetWindowShouldClose(m_Window, value);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		} else {
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const { return m_Data.VSync; }


	// GLFW Callbacks
	void WindowsWindow::GLFWErrorCallback(int error, const char* description) {
		SP_CORE_ERROR("GLFW Error: ", error, " ", description);
	}

	void WindowsWindow::GLFWWindowSizeCallback(GLFWwindow* window, int width, int height) {
		WindowsWindow* win = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		win->m_Data.frameBufferResized = true;
		win->m_Data.Width = width;
		win->m_Data.Height = height;

		GlobalEventDispatcher::Get().bufferEvent(std::make_shared<WindowResizeEvent>(width, height));
	}

	void WindowsWindow::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		switch (action) {
			case GLFW_PRESS: {
				GlobalEventDispatcher::Get().bufferEvent(std::make_shared<KeyPressedEvent>(key, false));
				break;
			}
			case GLFW_RELEASE: {
				GlobalEventDispatcher::Get().bufferEvent(std::make_shared<KeyReleasedEvent>(key));
				break;
			}
			/*case GLFW_REPEAT: {
				dispatcher.bufferEvent(std::make_shared<KeyPressedEvent>(key, true));
				break;
			}*/
		}
	}

	void WindowsWindow::GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		switch (action) {
			case GLFW_PRESS: {
				GlobalEventDispatcher::Get().bufferEvent(std::make_shared<MouseButtonPressedEvent>(button));
				break;
			}
			case GLFW_RELEASE: {
				GlobalEventDispatcher::Get().bufferEvent(std::make_shared<MouseButtonReleasedEvent>(button));
				break;
			}
		}
	}

	void WindowsWindow::GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
		GlobalEventDispatcher::Get().bufferEvent(std::make_shared<MouseMovedEvent>((float)xpos, (float)ypos));
	}

}