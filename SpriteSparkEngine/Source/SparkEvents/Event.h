#pragma once

#include "Sparkpch.h"

#include "SparkCore/HeaderFiles/Core.h"

namespace SpriteSpark {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseMoved, MouseScrolled, MouseButtonPressed, MouseButtonReleased
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event {
		
		friend class EventDispacher;

	public: 

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

	protected:

		bool m_Handled = false;

	};

    class EventDispatcher {

    public:

        using EventCallback = std::function<void(const std::shared_ptr<Event>&)>;

        EventDispatcher() = default;

        // Registrierung eines Listeners
        template<typename EventType, typename Func>
        void registerListener(Func&& func) {
            auto callback = [func](const std::shared_ptr<Event>& event) {
                if (auto derivedEvent = std::dynamic_pointer_cast<EventType>(event)) {
                    func(*derivedEvent);
                }
            };
            const std::type_index typeIndex(typeid(EventType));
            listeners[typeIndex].emplace_back(std::move(callback));
        }

        // Puffern von Events
        void bufferEvent(std::shared_ptr<Event> event) {
            if (!event) {
                SP_CORE_ERROR("Event is null!");
                return;
            }

            eventBuffer.emplace_back(std::move(event));

        }

        // Verarbeiten der gepufferten Events
        void updateEvents() {
            if (eventBuffer.empty()) return;

            for (const auto& event : eventBuffer) {
                try {
                    dispatch(event);
                }
                catch (const std::exception& e) {
                    SP_CORE_ERROR("Exception caught while dispatching event: ", e.what());
                }
                catch (...) {
                    SP_CORE_ERROR("Unknown exception caught while dispatching event.");
                }
            }
            eventBuffer.clear();
        }

    private:

        std::unordered_map<std::type_index, std::vector<EventCallback>> listeners;
        std::vector<std::shared_ptr<Event>> eventBuffer;

        void dispatch(const std::shared_ptr<Event>& event) {
            const std::type_index typeIndex(typeid(*event));
            auto it = listeners.find(typeIndex);
            if (it != listeners.end()) {
                for (const auto& listener : it->second) {
                    listener(event);
                }
            } else {
                SP_CORE_WARN("No listeners found for event type: ", event->GetName());
            }
        }
    };

	inline std::ostream& operator<< (std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

}