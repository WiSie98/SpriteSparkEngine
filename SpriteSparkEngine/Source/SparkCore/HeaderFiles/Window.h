#pragma once

#include "Sparkpch.h"

#include "vulkan/vulkan.h"

#include "SparkCore/HeaderFiles/Core.h"
#include "SparkEvents/Event.h"

namespace SpriteSpark {

	struct WindowProps {

		std::string Titel;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& titel = "SpriteSpark Engine", unsigned int  width = 1280, unsigned int height = 720) : Titel(titel), Width(width), Height(height) { }

	};

	class Window {

	public:

		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		virtual bool ShouldClose() = 0;

		virtual void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

	};

}