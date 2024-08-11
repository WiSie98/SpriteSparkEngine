#pragma once

#include "SparkCore/HeaderFiles/GlobalLoader.h"
#include "SparkCore/HeaderFiles/Sound.h"

namespace SpriteSpark {

	enum class CollisionType {
		STATIC,
		DYNAMIC,
		TRIGGER
	};

	struct Player {
		bool isOnGround = false;
		int lives = 1;
		int score = 0;

		Player(int lives = 1, int score = 0) : isOnGround(false), lives(lives), score(score) {}
	};

	struct RigidBody {
		glm::vec2 velocity{ 0.0f, 0.0f };
		glm::vec2 acceleration{ 10.0f, 0.0f };
		float mass = 1.0f;
		float drag = 0.5f;

		RigidBody() = default;

		RigidBody(const glm::vec2& velocity, const glm::vec2& acceleration, float mass, float drag)	: velocity(velocity), acceleration(acceleration), mass(mass), drag(drag) {}
	};

	struct Transform {
		glm::vec2 position{};
		glm::vec2 scale{ 1.0f, 1.0f };
		float rotation = 0;

		glm::mat2 mat2() {
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotationMat{ {c, s}, {-s, c} };

			glm::mat2 scaleMat{ {scale.x, 0.0f}, {0.0f, scale.y} };
			return rotationMat * scaleMat;
		}
	};

	struct Color {
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	struct Collision {

		Collision(float x, float y, float width, float height, CollisionType collisionType) : type(collisionType) {
			box.x = x;
			box.y = y;
			box.width = width;
			box.height = height;
		}

		Rect box;
		CollisionType type;
	};

	struct Sprite {

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;
		Sprite(Sprite&&) = default;
		Sprite& operator=(Sprite&&) = default;

		Sprite(std::unique_ptr<VulkanTexture>& texture, Rect rect) : texture(texture) {
			VulkanModel::Data modelData{};

			spriteRect = rect;

			float u_min, v_min, u_max, v_max;
			spriteRect.getUVs(u_min, v_min, u_max, v_max, static_cast<float>(texture->getWidth()), static_cast<float>(texture->getHeight()));

			modelData.vertices.push_back({ { 0.0f, spriteRect.height }, {}, {u_min, v_max}});					// Left/Bottom
			modelData.vertices.push_back({ { 0.0f, 0.0f }, {}, { u_min, v_min } });								// Left/Top
			modelData.vertices.push_back({ { spriteRect.width, spriteRect.height }, {}, { u_max, v_max } });	// Right/Bottom
			modelData.vertices.push_back({ { spriteRect.width, 0.0f }, {}, { u_max, v_min } });					// Right/Top

			modelData.indices = { 0, 1, 2, 1, 2, 3 };

			model = std::make_shared<VulkanModel>(GlobalLoader::Get().getDevice(), modelData);
		}

		Sprite(std::unique_ptr<VulkanTexture>& texture) : texture(texture) {
			VulkanModel::Data modelData{};

			spriteRect.width = static_cast<float>(texture->getWidth());
			spriteRect.height = static_cast<float>(texture->getHeight());

			float u_min, v_min, u_max, v_max;
			spriteRect.getUVs(u_min, v_min, u_max, v_max, static_cast<float>(texture->getWidth()), static_cast<float>(texture->getHeight()));

			modelData.vertices.push_back({ { 0.0f, static_cast<float>(texture->getHeight()) }, {}, { u_min, v_max } });											// Left/Bottom
			modelData.vertices.push_back({ { 0.0f, 0.0f }, {}, { u_min, v_min } });																				// Left/Top
			modelData.vertices.push_back({ { static_cast<float>(texture->getWidth()), static_cast<float>(texture->getHeight()) }, {}, { u_max, v_max } });		// Right/Bottom
			modelData.vertices.push_back({ { static_cast<float>(texture->getWidth()), 0.0f }, {}, { u_max, v_min } });											// Right/Top

			modelData.indices = { 0, 1, 2, 1, 2, 3 };

			model = std::make_shared<VulkanModel>(GlobalLoader::Get().getDevice(), modelData);
		}

		std::shared_ptr<VulkanModel> model;
		std::unique_ptr<VulkanTexture>& texture;
		Rect spriteRect;
	};

}