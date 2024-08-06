#include <SpriteSpark.h>

using namespace SpriteSpark;

class TestLayer : public Layer {

public:

    TestLayer() : Layer("Test") {}

    void OnInit(Camera& camera) override {
        sound.setFilepath("Sound/Overworld Theme - Super Mario World.mp3");

        GlobalLoader::LoadSprites(entityManager, tilemap, "Levels/vp_ts_metroidlevel.json", "Levels/vp_lv_metroidlevel.json");

        Entity player = entityManager.createEntity();

        entityManager.addComponent(player, Transform{ {0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f });
        entityManager.addComponent(player, Velocity{ 100.0f });
        entityManager.addComponent(player, Sprite{ playertiles, {0.0f, 0.0f, 16.0f, 32.0f} });
    }

    void OnUpdate(float deltaTime, Camera& camera) override {

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

        // ECS Test Start
        movementSystem.update(entityManager, camera, deltaTime);

        //Sprite* spr1 = entityManager.getComponent<Sprite>(entityManager.getEntityById(0));
        //Sprite* spr2 = entityManager.getComponent<Sprite>(entityManager.getEntityById(1));

        //SP_TRACE("Entity 1 TextureSize: (", spr1->texture->getHeight(), ", ", spr1->texture->getWidth(), ")");
        //SP_TRACE("Entity 2 TextureSize: (", spr2->texture->getHeight(), ", ", spr2->texture->getWidth(), ")");
        // ECS Test End

    }

    void OnRender(FrameInfo& frameInfo) override {
        spriteSystem.render(frameInfo, entityManager);
    }

    float cameraVelocity = 500.0f;

    std::unique_ptr<VulkanTexture> tilemap = GlobalLoader::LoadTexture("Textures/vp_ts_metroidlevel.png");
    std::unique_ptr<VulkanTexture> playertiles = GlobalLoader::LoadTexture("Textures/vp_sptsht_player.png");
    Sound sound;
    float volume = 1.0f;

    EntityManager entityManager;
    PlayerMovementSystem movementSystem;
    SpriteSystem spriteSystem;

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