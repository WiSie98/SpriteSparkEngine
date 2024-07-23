#include <SpriteSpark.h>

using namespace SpriteSpark;

class TestLayer : public Layer {

public:

    TestLayer() : Layer("Test") {

    }

    void OnUpdate(double deltaTime, Camera& camera, std::vector<GameObject>& gameObjects) override {

        if (Input::IsKeyPressed(Key::H)) {
            SP_TRACE("Key Pressed: H ");
        }
        
        if (Input::IsMouseButtonPressed(Mouse::Button0)) {
            SP_TRACE("MouseButton Pressed: LeftMB ");
        }

        // Test Code Start
        if (gameObjects[0].transform2d.translation.x > 2) {
            spriteVelocity = -0.05f;
        }
        else if (gameObjects[0].transform2d.translation.x < -2) {
            spriteVelocity = 0.05f;
        }

        gameObjects[0].transform2d.translation.x += spriteVelocity * deltaTime;
        gameObjects[0].transform2d.rotation += 0.05f * deltaTime;

        if (gameObjects[1].transform2d.translation.x > 2) {
            spriteVelocity1 = -0.05f;
        }
        else if (gameObjects[1].transform2d.translation.x < -2) {
            spriteVelocity1 = 0.05f;
        }

        gameObjects[1].transform2d.translation.x += spriteVelocity1 * deltaTime;
        gameObjects[1].transform2d.rotation += 0.05f * deltaTime;

        // Camera test
        if (Input::IsKeyPressed(Key::W)) {
            camera.setPosition({ camera.getPosition().x, camera.getPosition().y - cameraVelocity * deltaTime, 0.0f });
        }
        else if (Input::IsKeyPressed(Key::S)) {
            camera.setPosition({ camera.getPosition().x, camera.getPosition().y + cameraVelocity * deltaTime, 0.0f });
        }
        else if (Input::IsKeyPressed(Key::A)) {
            camera.setPosition({ camera.getPosition().x - cameraVelocity * deltaTime, camera.getPosition().y, 0.0f });
        }
        else if (Input::IsKeyPressed(Key::D)) {
            camera.setPosition({ camera.getPosition().x + cameraVelocity * deltaTime, camera.getPosition().y, 0.0f });
        }

        if (Input::IsKeyPressed(Key::Q)) {
            camera.setZoom(camera.getZoom() + cameraVelocity * deltaTime);
        }
        else if (Input::IsKeyPressed(Key::E)) {
            camera.setZoom(camera.getZoom() - cameraVelocity * deltaTime);
        }
        // Test Code End
    }

    void OnRender(RenderSystem& renderSystem, FrameInfo& frameInfo, std::vector<GameObject>& gameObjects) override {
        renderSystem.renderGameObjects(frameInfo, gameObjects);
    }

    float cameraVelocity = 0.5f;
    float spriteVelocity = 0.05f;
    float spriteVelocity1 = -0.05f;

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