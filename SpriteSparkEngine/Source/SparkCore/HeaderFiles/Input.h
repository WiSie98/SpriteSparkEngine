#pragma once

#include "SparkCore/HeaderFiles/KeyCodes.h"
#include "SparkCore/HeaderFiles/MouseCodes.h"

#include "SparkEvents/GlobalEventDispatcher.h"

namespace SpriteSpark {

    class Input {
    public:
        Input(EventDispatcher& dispatcher) {
            m_Dispatcher = &dispatcher;

            m_Dispatcher->registerListener<KeyPressedEvent>([this](const KeyPressedEvent& e) {
                m_KeyPressed[e.GetKeyCode()] = true;
                });

            m_Dispatcher->registerListener<KeyReleasedEvent>([this](const KeyReleasedEvent& e) {
                m_KeyReleased[e.GetKeyCode()] = true;
                });

            m_Dispatcher->registerListener<MouseButtonPressedEvent>([this](const MouseButtonPressedEvent& e) {
                m_MouseButtonPressed[e.GetMouseButton()] = true;
                });

            m_Dispatcher->registerListener<MouseButtonReleasedEvent>([this](const MouseButtonReleasedEvent& e) {
                m_MouseButtonReleased[e.GetMouseButton()] = true;
                });

            m_Dispatcher->registerListener<MouseMovedEvent>([this](const MouseMovedEvent& e) {
                m_MouseX = e.GetX();
                m_MouseY = e.GetY();
                });
        }

        static bool IsKeyPressed(KeyCode keycode) {
            return s_Instance->m_KeyPressed[keycode];
        }

        static bool IsKeyReleased(KeyCode keycode) {
            return s_Instance->m_KeyReleased[keycode];
        }

        static bool IsMouseButtonPressed(MouseCode button) {
            return s_Instance->m_MouseButtonPressed[button];
        }

        static bool IsMouseButtonReleased(MouseCode button) {
            return s_Instance->m_MouseButtonReleased[button];
        }

        static float GetMouseX() {
            return s_Instance->m_MouseX;
        }

        static float GetMouseY() {
            return s_Instance->m_MouseY;
        }

        static void Initialize(EventDispatcher& dispatcher) {
            s_Instance = new Input(dispatcher);
        }

        static void Clear() {
            s_Instance->m_KeyPressed.clear();
            s_Instance->m_KeyReleased.clear();
            s_Instance->m_MouseButtonPressed.clear();
            s_Instance->m_MouseButtonReleased.clear();
        }

    private:
        std::unordered_map<KeyCode, bool> m_KeyPressed;
        std::unordered_map<KeyCode, bool> m_KeyReleased;
        std::unordered_map<MouseCode, bool> m_MouseButtonPressed;
        std::unordered_map<MouseCode, bool> m_MouseButtonReleased;
        float m_MouseX = 0.0f, m_MouseY = 0.0f;

        EventDispatcher* m_Dispatcher;

        static Input* s_Instance;
    };

}