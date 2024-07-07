#include <SpriteSpark.h>

using namespace SpriteSpark;

class TestLayer : public SpriteSpark::Layer {

public:

    TestLayer() : Layer("Test") {

    }

    void OnUpdate() override {

        if (Input::IsKeyPressed(Key::H)) {
            SP_INFO("Key Pressed: H ");
        }

        if (Input::IsMouseButtonPressed(Mouse::Button0)) {
            SP_INFO("MouseButton Pressed: LeftMB ");
        }
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