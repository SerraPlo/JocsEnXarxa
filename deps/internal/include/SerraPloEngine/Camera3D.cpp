#include "Camera3D.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera3D::Init(int screenWidth, int screenHeight, glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) {
	this->viewportAspectRatio = GLfloat(screenWidth / screenHeight);
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}

glm::mat4 Camera3D::GetMatrix() const {
	return ComputeProjectionMatrix() * ComputeViewMatrix();
}

glm::mat4 Camera3D::ComputeViewMatrix() const {
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

glm::mat4 Camera3D::ComputeProjectionMatrix() const {
	return glm::perspective(this->FOV, this->viewportAspectRatio, this->near, this->far);
}

void Camera3D::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
	GLfloat velocity = this->movementSpeed * deltaTime;
	if (direction == FORWARD)	this->position += this->front * velocity;
	if (direction == BACKWARD)	this->position -= this->front * velocity;
	if (direction == LEFT)		this->position -= this->right * velocity;
	if (direction == RIGHT)		this->position += this->right * velocity;
}

void Camera3D::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch) {
	xoffset *= this->mouseSensitivity;
	yoffset *= this->mouseSensitivity;

	this->yaw += xoffset;
	this->pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch) {
		if (this->pitch > 89.0f) this->pitch = 89.0f;
		if (this->pitch < -89.0f) this->pitch = -89.0f;
	}
	// Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}

void Camera3D::ProcessMouseScroll(GLfloat yoffset) {
	if (this->FOV >= 1.0f && this->FOV <= 45.0f) this->FOV -= yoffset;
	if (this->FOV <= 1.0f) this->FOV = 1.0f;
	if (this->FOV >= 45.0f) this->FOV = 45.0f;
}

void Camera3D::updateCameraVectors() {
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->up = glm::normalize(glm::cross(this->right, this->front));
}
