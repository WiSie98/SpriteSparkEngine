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
            entities.each<Transform, RigidBody, Player, Sprite>([&](Entity entity, Transform& transform, RigidBody& rigidBody, Player& player, Sprite& sprite) {
                glm::vec2 inputForce(0.0f, 0.0f);

                if (Input::IsKeyPressed(Key::W) && player.isOnGround) {
                    inputForce.y = -3500000.0f;
                    player.isOnGround = false;
                }
                if (Input::IsKeyPressed(Key::A)) {
                    inputForce.x -= 40000.0f;
                    if (inputForce.x < -60000.0f) {
                        inputForce.x = -60000.0f;
                    }
                }
                if (Input::IsKeyPressed(Key::D)) {
                    inputForce.x += 40000.0f;
                    if (inputForce.x > 60000.0f) {
                        inputForce.x = 60000.0f;
                    }
                }

                if (Input::IsKeyPressed(Key::Escape)) {
                    transform.position.x = 240.0f;
                    transform.position.y = 3194.0f;
                }

                glm::vec2 acceleration = inputForce / rigidBody.mass;

                rigidBody.acceleration = acceleration * dt;

                camera.setPosition({ transform.position.x, transform.position.y, 0.0f });
                camera.setZoom(150.0f);
            });
        }
    };

    class PhysicsSystem : public System<PhysicsSystem> {
    public:

        void update(EntityManager& entities, float dt) {
            glm::vec2 gravity(0.0f, 200.0f);

            entities.each<Transform, RigidBody, Player>([&](Entity entity, Transform& transform, RigidBody& rigidBody, Player& player) {
                rigidBody.acceleration += gravity;
                rigidBody.velocity += rigidBody.acceleration * dt;
                rigidBody.velocity *= (1.0f - rigidBody.drag * dt);

                transform.position += rigidBody.velocity * dt;

                if (transform.position.y <= 0.0f) {
                    transform.position.y = 0.0f;
                    player.isOnGround = true;
                    rigidBody.velocity.y = 0.0f;
                } else {
                    player.isOnGround = false;
                }

                rigidBody.acceleration = glm::vec2(0.0f, 0.0f);
            });
        }

    };

    class CollisionSystem : public System<CollisionSystem> {
    public:

        void update(EntityManager& entities, float dt) {
            const int gridSize = 16;

            std::unordered_map<int, std::vector<Entity>> grid;

            entities.each<Transform, Collision>([&](Entity entity, Transform& transform, Collision& collision) {
                int gridX = static_cast<int>(transform.position.x / gridSize);
                int gridY = static_cast<int>(transform.position.y / gridSize);
                int gridKey = gridX + gridY * 1000;

                for (int offsetX = -1; offsetX <= 1; ++offsetX) {
                    for (int offsetY = -1; offsetY <= 1; ++offsetY) {
                        int adjacentGridKey = (gridX + offsetX) + (gridY + offsetY) * 1000;
                        grid[adjacentGridKey].push_back(entity);
                    }
                }
                });

            for (const auto& [key, entityList] : grid) {
                for (size_t i = 0; i < entityList.size(); ++i) {
                    auto entityA = entityList[i];
                    auto* transformA = entities.getComponent<Transform>(entityA);
                    auto* collisionA = entities.getComponent<Collision>(entityA);

                    for (size_t j = i + 1; j < entityList.size(); ++j) {
                        auto entityB = entityList[j];
                        auto* transformB = entities.getComponent<Transform>(entityB);
                        auto* collisionB = entities.getComponent<Collision>(entityB);

                        if (collisionA->type == CollisionType::STATIC && collisionB->type == CollisionType::STATIC) {
                            continue;
                        }

                        if (checkCollision(*transformA, *collisionA, *transformB, *collisionB)) {
                            resolveCollision(entityA, entityB, transformA, collisionA, transformB, collisionB, entities);
                        }
                    }
                }
            }
        }


    private:

        bool checkCollision(const Transform& transformA, const Collision& collisionA, const Transform& transformB, const Collision& collisionB) {
            Rect boxA = calculateBoundingBox(transformA, collisionA.box);
            Rect boxB = calculateBoundingBox(transformB, collisionB.box);

            return (boxA.x < boxB.x + boxB.width &&
                boxA.x + boxA.width > boxB.x &&
                boxA.y < boxB.y + boxB.height &&
                boxA.y + boxA.height > boxB.y);
        }

        Rect calculateBoundingBox(const Transform& transform, const Rect& localBox) {
            Rect worldBox;
            worldBox.x = transform.position.x + localBox.x * transform.scale.x;
            worldBox.y = transform.position.y + localBox.y * transform.scale.y;
            worldBox.width = localBox.width * transform.scale.x;
            worldBox.height = localBox.height * transform.scale.y;
            return worldBox;
        }

        void resolveCollision(Entity entityA, Entity entityB, Transform* transformA, Collision* collisionA, Transform* transformB, Collision* collisionB, EntityManager& entities) {
            auto* rigidBodyA = entities.getComponent<RigidBody>(entityA);
            auto* rigidBodyB = entities.getComponent<RigidBody>(entityB);
            auto* playerA = entities.getComponent<Player>(entityA);
            auto* playerB = entities.getComponent<Player>(entityB);

            Rect boxA = calculateBoundingBox(*transformA, collisionA->box);
            Rect boxB = calculateBoundingBox(*transformB, collisionB->box);

            float deltaX = (boxA.x + boxA.width / 2) - (boxB.x + boxB.width / 2);
            float deltaY = (boxA.y + boxA.height / 2) - (boxB.y + boxB.height / 2);
            float intersectX = (boxA.width / 2 + boxB.width / 2) - abs(deltaX);
            float intersectY = (boxA.height / 2 + boxB.height / 2) - abs(deltaY);

            if (intersectX > 0 && intersectY > 0) {
                if (intersectY < intersectX) {
                    if (collisionA->type == CollisionType::DYNAMIC) {
                        if (deltaY > 0) {
                            transformA->position.y = boxB.y - boxA.height;
                            rigidBodyA->velocity.y = 0.0f;
                            if (playerA) playerA->isOnGround = true;
                        } else {
                            transformA->position.y = boxB.y + boxB.height;
                            rigidBodyA->velocity.y = 0.0f;
                        }
                    } else if (collisionB->type == CollisionType::DYNAMIC) {
                        if (deltaY > 0) {
                            transformB->position.y = boxA.y - boxB.height;
                            rigidBodyB->velocity.y = 0.0f;
                            if (playerB) playerB->isOnGround = true;
                        } else {
                            transformB->position.y = boxA.y + boxA.height;
                            rigidBodyB->velocity.y = 0.0f;
                        }
                    }
                } else {
                    if (collisionA->type == CollisionType::DYNAMIC) {
                        if (deltaX > 0) {
                            transformA->position.x = boxB.x - boxA.width;
                        } else {
                            transformA->position.x = boxB.x + boxB.width;
                        }
                        rigidBodyA->velocity.x = 0.0f;
                    } else if (collisionB->type == CollisionType::DYNAMIC) {
                        if (deltaX > 0) {
                            transformB->position.x = boxA.x - boxB.width;
                        } else {
                            transformB->position.x = boxA.x + boxA.width;
                        }
                        rigidBodyB->velocity.x = 0.0f;
                    }
                }
            }
        }

    };
}