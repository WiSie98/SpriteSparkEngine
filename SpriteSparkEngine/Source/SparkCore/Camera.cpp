#include "Sparkpch.h"

#include "SparkCore/HeaderFiles/Camera.h"

namespace SpriteSpark {

	void Camera::setOrthographicProjection(float left, float right, float bottom, float top) {
		m_ProjectionMatrix = glm::ortho(left * m_Zoom, right * m_Zoom, bottom * m_Zoom, top * m_Zoom, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::setPerspectiveProjection(float fovy, float aspect, float znear, float zfar) {
		m_ProjectionMatrix = glm::perspective(fovy, aspect, znear, zfar);
	}

	void Camera::recalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}