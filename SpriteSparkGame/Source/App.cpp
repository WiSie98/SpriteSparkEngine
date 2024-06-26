#include <SpriteSpark.h>

using namespace SpriteSpark;

class TestLayer : public SpriteSpark::Layer {

public:

    TestLayer() : Layer("Test") {
    
        auto& dispatcher = GlobalEventDispatcher::Get();

        dispatcher.registerListener<KeyPressedEvent>([](const KeyPressedEvent& e) {
            SP_INFO("Key pressed: ", e.GetKeyCode());
        });

        dispatcher.registerListener<MouseMovedEvent>([](const MouseMovedEvent& e) {
            SP_INFO("Mouse moved to: ", e.GetX(), ", ", e.GetY());
        });

    }

    void OnUpdate() override {
        //SP_INFO("TestLayer::Update");
    }

};

class TestApp : public SpriteSpark::Application {

public:
    TestApp() {
        PushLayer(new TestLayer());
    }

    ~TestApp() { }
};

SpriteSpark::Application* SpriteSpark::CreateApplication() {
    return new TestApp();
}