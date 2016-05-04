#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>

// Default camera values
#define FOV 45.0f
#define NEAR 0.01f
#define FAR 1000.0f

class Camera {
public:
	glm::vec3 position	{ 0,0,0 };
	glm::vec3 up		{ 0,1,0 };
	glm::vec3 target	{ 0,0,0 };
	GLfloat viewportAspectRatio{ 1.0f };
	glm::mat4 viewMatrix;

	Camera() = default;
	void Init(int screenWidth, int screenHeight) { viewportAspectRatio = GLfloat(screenWidth / screenHeight); };
	glm::mat4 PVMatrix() const { return viewMatrix * ComputeViewMatrix(); };

	void Translate(const glm::vec3 &pos) { position = pos; viewMatrix = ComputeProjectionMatrix(); };
	void SetTarget(const glm::vec3 &pos) { target = pos; viewMatrix = ComputeProjectionMatrix(); };

private:
	glm::mat4 ComputeViewMatrix() const { return glm::lookAt(this->position, this->target, this->up); }
	glm::mat4 ComputeProjectionMatrix() const { return glm::perspective(glm::radians(FOV), this->viewportAspectRatio, NEAR, FAR); }
};

#undef FOV
#undef NEAR
#undef FAR