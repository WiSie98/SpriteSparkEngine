#pragma once

namespace SpriteSpark {

	class WindowResizeEvent : public Event {

	public:

		WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		unsigned int m_Width, m_Height;

	};

	class WindowCloseEvent : public Event {

	public:

		WindowCloseEvent(bool windowClose) : m_WindowClose(windowClose) {}

		bool GetShouldWindowClose() const { return m_WindowClose; }

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		bool m_WindowClose = false;

	};
}