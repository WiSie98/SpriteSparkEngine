#pragma once

#include "Platform/Vulkan/HeaderFiles/VulkanModel.h"
#include "Platform/Vulkan/HeaderFiles/VulkanTexture.h"

namespace SpriteSpark {

	struct Transform2dComponent {
		glm::vec2 translation{};
		glm::vec2 scale{ 1.0f, 1.0f };
		float rotation;

		glm::mat2 mat2() {
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotationMat{ {c, s}, {-s, c} };

			glm::mat2 scaleMat{ {scale.x, 0.0f}, {0.0f, scale.y} };
			return rotationMat * scaleMat;
		}
	};

	class GameObject {

	public:

		using id_t = unsigned int;

		static GameObject createGameObject() {
			static id_t currentId = 0;
			return GameObject{currentId++};
		}

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		const id_t getId() { return m_Id; }

		std::shared_ptr<VulkanModel> model;
		glm::vec4 color{};
		Transform2dComponent transform2d{};
		std::unique_ptr<VulkanTexture> texture;

	private:
		
		GameObject(id_t objId) : m_Id{} {}

		id_t m_Id;

	};

}