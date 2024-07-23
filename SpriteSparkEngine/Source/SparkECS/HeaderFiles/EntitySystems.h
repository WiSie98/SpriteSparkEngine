#pragma once

#include "SparkECS/HeaderFiles/EntityComponents.h"
#include "SparkECS/HeaderFiles/EntityManager.h"

template <typename Derived>
class System {
public:
    void update(EntityManager& entities, float dt) {
        static_cast<Derived*>(this)->update(entities, dt);
    }
};

class MovementSystem : public System<MovementSystem> {
public:
    void update(EntityManager& entities, float dt) {
        entities.each<Position, Direction>([dt](Entity entity, Position& position, Direction& direction) {
            position.x += direction.x * dt;
            position.y += direction.y * dt;
            });
    }
};

class CollisionSystem : public System<CollisionSystem> {
public:
    void update(EntityManager& entities, float dt) {

    }
};
