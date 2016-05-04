#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
#define YAW -90.0f
#define PITCH 0.0f
#define SPEED 3.0f
#define SENSITIVTY 0.25f
#define FOV 45.0f
#define NEAR 0.01f
#define FAR 1000.0f

class Camera {
public:
	glm::vec3 position{ 0,0,0 };
	glm::vec3 front{ 0,0,0 };
	glm::vec3 up{ 0,1,0 };
	glm::vec3 target{ 0,0,0 };
	GLfloat viewportAspectRatio{ 1.0f };

	Camera() = default;
	void Init(int screenWidth, int screenHeight) { viewportAspectRatio = GLfloat(screenWidth / screenHeight); };
	glm::mat4 PVMatrix() { return ComputeProjectionMatrix() * ComputeViewMatrix(); };
	
	void SetTarget(const glm::vec3 &pos) { target = pos; };
	void UpdateVec() {

	}

private:
	glm::mat4 ComputeViewMatrix() const {
		return glm::lookAt(this->position, target, this->up);
	}

	glm::mat4 ComputeProjectionMatrix() const {
		return glm::perspective(FOV, this->viewportAspectRatio, NEAR, FAR);
	}
};

#undef YAW
#undef PITCH
#undef SPEED
#undef SENSITIVTY
#undef ZOOM
#undef NEAR
#undef FAR