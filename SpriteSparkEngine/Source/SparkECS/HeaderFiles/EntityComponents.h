#pragma once

#include "SparkCore/HeaderFiles/GlobalLoader.h"

namespace SpriteSpark {

	struct Position {
		float x = 0, y = 0;
	};

	struct Direction {
		float x = 0, y = 0;
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

	struct Sprite {

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;
		Sprite(Sprite&&) = default;
		Sprite& operator=(Sprite&&) = default;

		Sprite(std::string& filePath, Rect rect) {
			VulkanModel::Data modelData{};

			texture = GlobalLoader::Get().LoadTexture(filePath);
			spriteRect = rect;

			float textureSize = static_cast<float>(texture->getHeight());

			float u_min, v_min, u_max, v_max;
			spriteRect.getUVs(u_min, v_min, u_max, v_max, static_cast<float>(texture->getWidth()), static_cast<float>(texture->getHeight()));

			modelData.vertices.push_back({ { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { u_min, v_max } });     // Left/Bottom
			modelData.vertices.push_back({ { 0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { u_min, v_min } });    // Left/Top
			modelData.vertices.push_back({ { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { u_max, v_max } });    // Right/Bottom
			modelData.vertices.push_back({ { -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { u_max, v_min } });   // Right/Top

			modelData.indices = { 0, 1, 2, 1, 2, 3 };

			model = std::make_shared<VulkanModel>(GlobalLoader::Get().getDevice(), modelData);
		}

		Sprite(std::string& filePath) {
			VulkanModel::Data modelData{};

			texture = GlobalLoader::Get().LoadTexture(filePath);

			float textureSize = static_cast<float>(texture->getHeight());
			spriteRect.width = textureSize;
			spriteRect.height = textureSize;

			float u_min, v_min, u_max, v_max;
			spriteRect.getUVs(u_min, v_min, u_max, v_max, static_cast<float>(texture->getWidth()), static_cast<float>(texture->getHeight()));

			modelData.vertices.push_back({ { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { u_min, v_max } });     // Left/Bottom
			modelData.vertices.push_back({ { 0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { u_min, v_min } });    // Left/Top
			modelData.vertices.push_back({ { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { u_max, v_max } });    // Right/Bottom
			modelData.vertices.push_back({ { -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { u_max, v_min } });   // Right/Top

			modelData.indices = { 0, 1, 2, 1, 2, 3 };

			model = std::make_shared<VulkanModel>(GlobalLoader::Get().getDevice(), modelData);
		}

		std::shared_ptr<VulkanModel> model;
		std::unique_ptr<VulkanTexture> texture;
		Rect spriteRect;
	};

}