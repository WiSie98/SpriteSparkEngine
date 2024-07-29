#pragma once

#include "SparkCore/HeaderFiles/KeyCodes.h"
#include "SparkCore/HeaderFiles/MouseCodes.h"

#include "SparkEvents/GlobalEventDispatcher.h"

namespace SpriteSpark {

    class Input {
    public:
        Input() {
            GlobalEventDispatcher::Get().registerListener<KeyPressedEvent>([this](const KeyPressedEvent& e) {
                m_KeyPressed[e.GetKeyCode()] = true;
                });

            GlobalEventDispatcher::Get().registerListener<KeyReleasedEvent>([this](const KeyReleasedEvent& e) {
                m_KeyReleased[e.GetKeyCode()] = true;
                });

            GlobalEventDispatcher::Get().registerListener<MouseButtonPressedEvent>([this](const MouseButtonPressedEvent& e) {
                m_MouseButtonPressed[e.GetMouseButton()] = true;
                });

            GlobalEventDispatcher::Get().registerListener<MouseButtonReleasedEvent>([this](const MouseButtonReleasedEvent& e) {
                m_MouseButtonReleased[e.GetMouseButton()] = true;
                });

            GlobalEventDispatcher::Get().registerListener<MouseMovedEvent>([this](const MouseMovedEvent& e) {
                m_MouseX = e.GetX();
                m_MouseY = e.GetY();
                });
        }

        ~Input() {}

        static bool IsKeyPressed(const KeyCode& keycode) {
            return Get().m_KeyPressed[keycode];
        }

        static bool IsKeyReleased(const KeyCode& keycode) {
            return Get().m_KeyReleased[keycode];
        }

        static bool IsMouseButtonPressed(const MouseCode& button) {
            return Get().m_MouseButtonPressed[button];
        }

        static bool IsMouseButtonReleased(const MouseCode& button) {
            return Get().m_MouseButtonReleased[button];
        }

        static float GetMouseX() {
            return Get().m_MouseX;
        }

        static float GetMouseY() {
            return Get().m_MouseY;
        }

        static void Clear() {
            for (const auto& key : Get().m_KeyReleased) {
                if (key.second) {
                    Get().m_KeyPressed[key.first] = false;
                }
            }

            for (const auto& key : Get().m_MouseButtonReleased) {
                if (key.second) {
                    Get().m_MouseButtonPressed[key.first] = false;
                }
            }

            Get().m_KeyReleased.clear();
            Get().m_MouseButtonReleased.clear();
        }

    private:
        std::unordered_map<KeyCode, bool> m_KeyPressed;
        std::unordered_map<KeyCode, bool> m_KeyReleased;
        std::unordered_map<MouseCode, bool> m_MouseButtonPressed;
        std::unordered_map<MouseCode, bool> m_MouseButtonReleased;
        float m_MouseX = 0.0f, m_MouseY = 0.0f;

        inline static Input& Get() {
            static Input input;
            return input;
        }
    };

}