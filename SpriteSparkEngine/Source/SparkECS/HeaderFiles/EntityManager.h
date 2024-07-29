#pragma once

#include "Sparkpch.h"

class Entity {
public:
    std::uint64_t id;

    Entity(std::uint64_t id) : id(id) {}

    bool operator==(const Entity& other) const {
        return id == other.id;
    }
};

namespace std {
    template <>
    struct hash<Entity> {
        std::size_t operator()(const Entity& entity) const noexcept {
            return std::hash<std::uint64_t>{}(entity.id);
        }
    };
}

class IComponent {

public:

    virtual ~IComponent() = default;
};

template <typename T>
class Component : public IComponent {

public:
    T data;

    // Perfect forwarding constructor
    template <typename U>
    Component(U&& componentData) : data(std::forward<U>(componentData)) {}
};

class EntityManager {
public:
    Entity createEntity() {
        std::uint64_t id;
        if (!m_RecycledIds.empty()) {
            id = m_RecycledIds.back();
            m_RecycledIds.pop_back();
        }
        else {
            id = m_NextId++;
        }
        Entity entity(id);
        m_Entities.insert(entity);
        return entity;
    }

    Entity getEntityById(std::uint64_t id) {
        auto it = m_Entities.find(Entity(id));
        if (it != m_Entities.end()) {
            return *it;
        }
        throw std::runtime_error("Entity not found!");
    }

    void deleteEntity(Entity entity) {
        deleteEntityById(entity.id);
    }

    void deleteEntityById(std::uint64_t id) {
        m_Entities.erase(Entity(id));
        m_RecycledIds.push_back(id);

        // Remove associated components
        for (auto& [typeHash, componentVector] : m_Components) {
            if (id < componentVector.size()) {
                componentVector[id].reset();
            }
        }
    }

    template <typename T>
    void addComponent(Entity entity, T&& component) {
        auto& componentVector = m_Components[typeid(T).hash_code()];
        if (entity.id >= componentVector.size()) {
            componentVector.resize(entity.id + 1);
        }
        componentVector[entity.id] = std::make_shared<Component<T>>(std::forward<T>(component));
    }

    template <typename T>
    T* getComponent(Entity entity) {
        return getComponentById<T>(entity.id);
    }

    template <typename T>
    T* getComponentById(std::uint64_t id) {
        auto it = m_Components.find(typeid(T).hash_code());
        if (it != m_Components.end() && id < it->second.size()) {
            return it->second[id] ? &std::static_pointer_cast<Component<T>>(it->second[id])->data : nullptr;
        }
        return nullptr;
    }

    template <typename... Components, typename Func>
    void each(Func func) {
        for (const auto& entity : m_Entities) {
            if (hasComponentsById<Components...>(entity.id)) {
                func(entity, *getComponentById<Components>(entity.id)...);
            }
        }
    }

private:

    std::uint64_t m_NextId = 0;
    std::unordered_set<Entity> m_Entities;
    std::vector<std::uint64_t> m_RecycledIds;
    std::unordered_map<std::size_t, std::vector<std::shared_ptr<IComponent>>> m_Components;

    template <typename T>
    bool hasComponentById(std::uint64_t id) {
        auto it = m_Components.find(typeid(T).hash_code());
        return it != m_Components.end() && id < it->second.size() && it->second[id];
    }

    template <typename... Components>
    bool hasComponentsById(std::uint64_t id) {
        return (hasComponentById<Components>(id) && ...);
    }
};
