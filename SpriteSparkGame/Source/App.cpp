#include <SpriteSpark.h>

using namespace SpriteSpark;

class TestLayer : public Layer {

public:

    TestLayer() : Layer("Test") {}

    void OnInit(Camera& camera) override {
        sound.setFilepath("Sound/Theme of Samus Aran, Space Warrior - Super Smash Bros. Ultimate.mp3");

        GlobalLoader::LoadSprites(entityManager, 3, tilemap, "Levels/vp_ts_metroidlevel.json", "Levels/vp_lv_metroidlevel.json");
        GlobalLoader::LoadCollider(entityManager, 3, "Levels/vp_ts_metroidlevel.json", "Levels/vp_lv_metroidlevel.json");
        GlobalLoader::LoadCollider(entityManager, 4, "Levels/vp_ts_metroidlevel.json", "Levels/vp_lv_metroidlevel.json");

        Entity player = entityManager.createEntity();

        entityManager.addComponent(player, Player{});
        entityManager.addComponent(player, Transform{ {240.0f, 3194.0f}, {1.0f, 1.0f}, 0.0f });
        entityManager.addComponent(player, RigidBody{});
        entityManager.addComponent(player, Collision{ 0.0f, 0.0f, 16.0f, 32.0f, CollisionType::DYNAMIC });
        entityManager.addComponent(player, Sprite{ playerSprite, {0.0f, 0.0f, 16.0f, 32.0f} });

        sound.play(true);
        sound.setVolume(volume);
    }

    void OnUpdate(float deltaTime, Camera& camera) override {

        // Sound Test Start

        if (Input::IsKeyPressed(Key::Up)) {
            if (volume < 1.0f) volume = volume + 0.01f;
            SP_APP_INFO("Lautstärke: ", volume);
            sound.setVolume(volume);
        } else if (Input::IsKeyPressed(Key::Down)) {
            if (volume > 0.0f) volume = volume - 0.01f;
            SP_APP_INFO("Lautstärke: ", volume);
            sound.setVolume(volume);
        }

        // Sound Test End

        // ECS Test Start
        movementSystem.update(entityManager, camera, deltaTime);
        physicsSystem.update(entityManager, deltaTime);
        collisionSystem.update(entityManager, deltaTime);
        // ECS Test End

    }

    void OnRender(FrameInfo& frameInfo) override {
        spriteSystem.render(frameInfo, entityManager);
    }

    float cameraVelocity = 500.0f;

    std::unique_ptr<VulkanTexture> tilemap = GlobalLoader::LoadTexture("Textures/vp_ts_metroidlevel.png");
    std::unique_ptr<VulkanTexture> playerSprite = GlobalLoader::LoadTexture("Textures/vp_sptsht_player.png");
    Sound sound;
    float volume = 0.5f;

    EntityManager entityManager;
    PlayerMovementSystem movementSystem;
    SpriteSystem spriteSystem;
    CollisionSystem collisionSystem;
    PhysicsSystem physicsSystem;

};

class TestApp : public SpriteSpark::Application {

public:
    TestApp() {
        PushLayer(new TestLayer());
    }

    ~TestApp() {}
};

SpriteSpark::Application* SpriteSpark::CreateApplication() {
    return new TestApp();
}