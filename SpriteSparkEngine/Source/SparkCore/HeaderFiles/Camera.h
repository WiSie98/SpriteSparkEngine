#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace SpriteSpark {

	class Camera {

	public:

		void setOrthographicProjection(float left, float right, float bottom, float top);
		void setPerspectiveProjection(float fovy, float aspect, float znear, float zfar);

		void setPosition(const glm::vec3& position) { m_Position = position; recalculateViewMatrix(); }
		void setRotation(const float rotation) { m_Rotation = glm::radians(rotation); recalculateViewMatrix(); }
		void setZoom(const float zoom) { m_Zoom = zoom, recalculateViewMatrix(); }

		const glm::vec3& getPosition() const { return m_Position; }
		float getRotation() const { return m_Rotation; }
		float getZoom() const { return m_Zoom; }
		const glm::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& getViewProjectionMatrix () const { return m_ViewProjectionMatrix; }

	private:

		void recalculateViewMatrix();

		glm::mat4 m_ProjectionMatrix{ 1.0f };
		glm::mat4 m_ViewMatrix{ 1.0f };
		glm::mat4 m_ViewProjectionMatrix{ 1.0f };

		glm::vec3 m_Position{ 0.0f };
		float m_Rotation = 0.0f;
		float m_Zoom = 1.0f;

	};

}