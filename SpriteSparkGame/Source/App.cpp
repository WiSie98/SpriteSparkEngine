#include <SpriteSpark.h>

using namespace SpriteSpark;

class TestLayer : public Layer {

public:

    TestLayer() : Layer("Test") {}

    void OnInit() override {
        sound.setFilepath("Sound/Overworld Theme - Super Mario World.mp3");

        Entity entity1 = entityManager.createEntity();
        Entity entity2 = entityManager.createEntity();

        std::string file = "Textures/Test.png";

        entityManager.addComponent(entity1, Position{ 0, 0 });
        entityManager.addComponent(entity1, Direction{ 1, 1 });
        entityManager.addComponent(entity1, Transform{ {1.0f, 1.0f}, {1.0f, 1.0f}, 0.0f });
        entityManager.addComponent(entity1, Sprite(file, {0, 0, 16, 16}));
        entityManager.addComponent(entity2, Position{ 0, 0 });
        entityManager.addComponent(entity2, Direction{ -1, -1 });
    }

    void OnUpdate(float deltaTime, Camera& camera, std::vector<GameObject>& gameObjects) override {

        // Sound Test Start
        if (Input::IsKeyPressed(Key::H)) {
            sound.play();
        }

        if (Input::IsKeyPressed(Key::Up)) {
            if (volume < 1.0f) volume += 0.0001f;
            sound.setVolume(volume);
        } else if (Input::IsKeyPressed(Key::Down)) {
            if (volume > 0.0f) volume -= 0.0001f;
            sound.setVolume(volume);
        }

        if (Input::IsKeyPressed(Key::P)) {
            sound.pause();
        }
        // Sound Test End
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

        // ECS Test Start
        movementSystem.update(entityManager, deltaTime);

        //Position* pos1 = entityManager.getComponent<Position>(entityManager.getEntityById(0));
        //Position* pos2 = entityManager.getComponent<Position>(entityManager.getEntityById(1));

        //SP_TRACE("Entity 1 Position: (", pos1->x, ", ", pos1->y, ")");
        //SP_TRACE("Entity 2 Position: (", pos2->x, ", ", pos2->y, ")");
        // ECS Test End

    }

    void OnRender(RenderSystem& renderSystem, FrameInfo& frameInfo, std::vector<GameObject>& gameObjects) override {
        renderSystem.renderGameObjects(frameInfo, gameObjects);
    }

    float cameraVelocity = 0.5f;
    float spriteVelocity = 0.05f;
    float spriteVelocity1 = -0.05f;

    Sound sound;
    float volume = 1.0f;

    EntityManager entityManager;
    MovementSystem movementSystem;

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