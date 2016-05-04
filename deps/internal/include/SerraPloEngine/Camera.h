#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <GL/glew.h>

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
#define ZOOM 45.0f
#define NEAR 0.01f
#define FAR 1000.0f

class Camera {
	glm::vec3 position	{ 0,0,0 };
	glm::vec3 front		{ 0,0,0 };
	glm::vec3 up		{ 0,0,0 };
	glm::vec3 right		{ 0,0,0 };
	glm::vec3 worldUp	{ 0,0,0 };

	GLfloat yaw, pitch;

	GLfloat movementSpeed{ SPEED };
	GLfloat mouseSensitivity{ SENSITIVTY };
	GLfloat FOV{ ZOOM };
	GLfloat viewportAspectRatio{ 1.0f };
	GLfloat far{ FAR };
	GLfloat near{ NEAR };
public:
	glm::mat4 PVMatrix;

	Camera() = default;

	void Init(int screenWidth, int screenHeight, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

	void Update() { PVMatrix = ComputeProjectionMatrix() * ComputeViewMatrix(); };
	void SetPosition(const glm::vec3 &pos) { position = pos; Update(); };
	void SetTarget(const glm::vec3 &pos) {
		this->front = pos - this->position;
		this->front = glm::normalize(front);
		Update(); 
	};

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yoffset);

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors();
	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 ComputeViewMatrix() const;
	glm::mat4 ComputeProjectionMatrix() const;
};

#undef YAW
#undef PITCH
#undef SPEED
#undef SENSITIVTY
#undef ZOOM
#undef NEAR
#undef FAR