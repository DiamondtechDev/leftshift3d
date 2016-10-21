#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed_(SPEED), mouseSensitivity_(SENSITIVTY), zoom_(ZOOM)
{
	position_ = position;
	worldUp_ = up;
	yaw_ = yaw;
	pitch_ = pitch;
	updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed_(SPEED), mouseSensitivity_(SENSITIVTY), zoom_(ZOOM)
{
	position_ = glm::vec3(posX, posY, posZ);
	worldUp_ = glm::vec3(upX, upY, upZ);
	yaw_ = yaw;
	pitch_ = pitch;
	updateCameraVectors();
}

Camera::~Camera()
{
	
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed_ * deltaTime;

	if (direction == FORWARD)
		position_ += front_ * velocity;
	if (direction == BACKWARD)
		position_ -= front_ * velocity;
	if (direction == LEFT)
		position_ -= right_ * velocity;
	if (direction == RIGHT)
		position_ += right_ * velocity;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
{
	xoffset *= mouseSensitivity_;
	yoffset *= mouseSensitivity_;

	yaw_   += xoffset;
	pitch_ += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch_ > 89.0f)
			pitch_ = 89.0f;
		if (pitch_ < -89.0f)
			pitch_ = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	if (zoom_ >= 44.0f && zoom_ <= 45.0f)
		zoom_ -= yoffset;
	
	if (zoom_ <= 44.0f)
		zoom_ = 44.0f;

	if (zoom_ >= 45.0f)
		zoom_ = 45.0f;
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_ = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	right_ = glm::normalize(glm::cross(front_, worldUp_));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up_    = glm::normalize(glm::cross(right_, front_));
}
