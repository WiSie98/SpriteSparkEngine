#pragma once

#include "SparkCore/HeaderFiles/Input.h"
#include "SparkCore/HeaderFiles/Camera.h"

#include "SparkECS/HeaderFiles/EntityComponents.h"
#include "SparkECS/HeaderFiles/EntityManager.h"

namespace SpriteSpark {

    template <typename Derived>
    class System {
    public:
        void update(EntityManager& entities, float dt) {
            static_cast<Derived*>(this)->update(entities, dt);
        }
    };

    class PlayerMovementSystem : public System<PlayerMovementSystem> {
    public:
        void update(EntityManager& entities, Camera& camera, float dt) {
            entities.each<Transform, Velocity>([&](Entity entity, Transform& transform, Velocity& velocity) {
                if (Input::IsKeyPressed(Key::W)) {
                    transform.position.y -= velocity.velocity * dt;
                }
                else if (Input::IsKeyPressed(Key::S)) {
                    transform.position.y += velocity.velocity * dt;
                }
                else if (Input::IsKeyPressed(Key::A)) {
                    transform.position.x -= velocity.velocity * dt;
                }
                else if (Input::IsKeyPressed(Key::D)) {
                    transform.position.x += velocity.velocity * dt;
                }

                camera.setPosition({ transform.position.x, transform.position.y, 0.0f });
                camera.setZoom(150.0f);
            });
        }
    };

    class CollisionSystem : public System<CollisionSystem> {
    public:
        void update(EntityManager& entities, float dt) {

        }
    };

}