#include "Camera2D.h"
#include <glm/gtc/matrix_transform.inl>

namespace SerraPlo {

Camera2D::Camera2D() :
	m_screenWidth(600),
	m_screenHeight(600),
	m_needsMatrixUpdate(true), 
	m_scale(1.0f), 
	m_position(0.0f, 0.0f),
	m_cameraMatrix(1.0f),
	m_orthoMatrix(1.0f)
{}

Camera2D::Camera2D(int screenWidth, int screenHeight) :
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight),
	m_needsMatrixUpdate(true),
	m_scale(1.0f),
	m_position(0.0f, 0.0f),
	m_cameraMatrix(1.0f),
	m_orthoMatrix(1.0f)
{
	m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_screenWidth), 0.0f, static_cast<float>(m_screenHeight));
}

void Camera2D::init(int screenWidth, int screenHeight) {
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_screenWidth), 0.0f, static_cast<float>(m_screenHeight));
}

void Camera2D::update() {
	if (m_needsMatrixUpdate) {
		m_cameraMatrix = glm::translate(m_orthoMatrix, glm::vec3(-m_position.x+ m_screenWidth / 2, -m_position.y+ m_screenHeight / 2, 0.0f));
		m_cameraMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale, m_scale, 0.0f))*m_cameraMatrix;
		m_needsMatrixUpdate = false;
	}
}

glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) const
{
	screenCoords.y = m_screenHeight - screenCoords.y; //invert y direction
	screenCoords -= glm::vec2(m_screenWidth / 2, m_screenHeight / 2); //make 0 center of screen
	screenCoords /= m_scale; //scale coordinates
	screenCoords += m_position; //add camera position
	return screenCoords;
}

bool Camera2D::isBoxInView(const glm::vec2 & position, const glm::vec2 & dimensions) const
{
	glm::vec2 scaledScreenDimensions{ glm::vec2(m_screenWidth, m_screenHeight) / (m_scale*0.9f) }; //*0.9f error margin for correct visualization

	const auto MIN_DISTANCE_X{ dimensions.x * 0.5f + scaledScreenDimensions.x * 0.5f };
	const auto MIN_DISTANCE_Y{ dimensions.y * 0.5f + scaledScreenDimensions.y * 0.5f };

	auto centerPos{ m_position + (dimensions * 0.5f) };
	auto distVec{centerPos - position};

	auto xDepth{ MIN_DISTANCE_X - abs(distVec.x) };
	auto yDepth{ MIN_DISTANCE_Y - abs(distVec.y) };

	if (xDepth > 0 && yDepth > 0) return true;
	return false;
}
}
