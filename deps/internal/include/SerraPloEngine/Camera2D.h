#pragma once
#include <glm/glm.hpp>

namespace SerraPlo {

class Camera2D 
{
	int m_screenWidth, m_screenHeight;
	bool m_needsMatrixUpdate;
	float m_scale;
	glm::vec2 m_position;
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_orthoMatrix;
public:
	explicit Camera2D();
	explicit Camera2D(int screenWidth, int screenHeight);
	~Camera2D() = default;

	void init(int screenWidth, int screenHeight);
	void update();
	glm::vec2 convertScreenToWorld(glm::vec2 screenCoords) const;
	bool isBoxInView(const glm::vec2 &position, const glm::vec2 &dimensions) const;

	//setters
	void setPosition(const glm::vec2& newPos) { m_position = newPos; m_needsMatrixUpdate = true; };
	void setScale(float newScale) { m_scale = newScale; m_needsMatrixUpdate = true; };

	//getters
	glm::vec2 getPosition() const { return m_position; };
	float getScale() const { return m_scale; };
	glm::mat4 getCameraMatrix() const { return m_cameraMatrix; };
};

}

