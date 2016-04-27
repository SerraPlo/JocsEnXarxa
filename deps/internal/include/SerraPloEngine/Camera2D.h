#pragma once
#include <glm/glm.hpp>

namespace SerraPlo {

	class Camera2D {
		int m_screenWidth, m_screenHeight;	// Screen dimensions
		bool m_needsMatrixUpdate;			// Check if camera has to be transformed, so then updated
		float m_scale;						// How the world would be scaled due to camera matrix transformation
		glm::vec2 m_position;				// 2D position of the camera in world
		glm::mat4 m_cameraMatrix;			// Projection matrix to be transformed
		glm::mat4 m_orthoMatrix;			// Orthographic matrix for translation
	public:
		glm::vec2 mouseScreenCoords;		// Mouse window coordinates transformed to world position coordinates

		explicit Camera2D();
		explicit Camera2D(int screenWidth, int screenHeight);
		~Camera2D() = default;

		void init(int screenWidth, int screenHeight);
		void update(glm::vec2 screenCoords = {0,0});
		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords) const;
		bool isBoxInView(const glm::vec2 &position, const glm::vec2 &dimensions) const;

		//setters
		void setPosition(const glm::vec2& newPos)	{ m_position = newPos; m_needsMatrixUpdate = true; };
		void setScale(float newScale)				{ m_scale = newScale; m_needsMatrixUpdate = true; };

		//getters
		glm::vec2 getPosition() const		{ return m_position; };
		float getScale() const				{ return m_scale; };
		glm::mat4 getCameraMatrix() const	{ return m_cameraMatrix; };
	};

}

