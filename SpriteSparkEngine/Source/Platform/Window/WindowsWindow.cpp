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

		uint32_t extensionCount = 0;

		SP_CORE_INFO("Creating window ", props.Titel, ", Resolution: ", props.Width, "x", props.Height);

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

		glfwSetWindowSizeCallback(m_Window, GLFWWindowSizeCallback);
		glfwSetKeyCallback(m_Window, GLFWKeyCallback);
		glfwSetMouseButtonCallback(m_Window, GLFWMouseButtonCallback);
		glfwSetCursorPosCallback(m_Window, GLFWCursorPosCallback);

		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		SP_CORE_INFO(extensionCount, " extensions supported.");
		*/
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

		auto& dispatcher = GlobalEventDispatcher::Get();
		dispatcher.bufferEvent(std::make_shared<WindowResizeEvent>(width, height));
	}

	void WindowsWindow::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		WindowsWindow* win = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));

		auto& dispatcher = GlobalEventDispatcher::Get();
		switch (action) {
			case GLFW_PRESS: {
				dispatcher.bufferEvent(std::make_shared<KeyPressedEvent>(key, false));
				break;
			}
			case GLFW_RELEASE: {
				dispatcher.bufferEvent(std::make_shared<KeyReleasedEvent>(key));
				break;
			}
			case GLFW_REPEAT: {
				dispatcher.bufferEvent(std::make_shared<KeyPressedEvent>(key, true));
				break;
			}
		}
	}

	void WindowsWindow::GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		WindowsWindow* win = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));

		auto& dispatcher = GlobalEventDispatcher::Get();
		switch (action) {
			case GLFW_PRESS: {
				dispatcher.bufferEvent(std::make_shared<MouseButtonPressedEvent>(button));
				break;
			}
			case GLFW_RELEASE: {
				dispatcher.bufferEvent(std::make_shared<MouseButtonReleasedEvent>(button));
				break;
			}
		}
	}

	void WindowsWindow::GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
		//SP_CORE_INFO("GLFWCursorPosCallback triggered with posX: ", xpos, ", posY: ", ypos);
		WindowsWindow* win = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		if (win == nullptr) {
			SP_CORE_ERROR("Window pointer is null!");
			return;
		}

		auto& dispatcher = GlobalEventDispatcher::Get();
		dispatcher.bufferEvent(std::make_shared<MouseMovedEvent>((float)xpos, (float)ypos));
	}

}