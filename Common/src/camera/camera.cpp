

#include "camera/camera.h"
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
	: m_position(position)
	, m_worldUp(up)
	, m_yaw(yaw)
	, m_pitch(pitch)
	, m_front(glm::vec3(0.0f, 0.0f, -1.0f))
	, m_movementSpeed(2.5f)
	, m_mouseSensitivity(0.1f)
	, m_zoom(45.0f)
{
	updateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: m_position(glm::vec3(posX, posY, posZ))
	, m_worldUp(glm::vec3(upX, upY, upZ))
	, m_yaw(yaw)
	, m_pitch(pitch)
	, m_front(glm::vec3(0.0f, 0.0f, -1.0f))
	, m_movementSpeed(2.5f)
	, m_mouseSensitivity(0.1f)
	, m_zoom(45.0f)
{
	updateCameraVectors();
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = m_movementSpeed * deltaTime;
	if (direction == Camera_Movement::FORWARD)
		m_position += m_front * velocity;
	if (direction == Camera_Movement::BACKWARD)
		m_position -= m_front * velocity;
	if (direction == Camera_Movement::LEFT)
		m_position -= m_right * velocity;
	if (direction == Camera_Movement::RIGHT)
		m_position += m_right * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		m_zoom -= yoffset;
	if (m_zoom <= 1.0f)
		m_zoom = 1.0f;
	if (m_zoom >= 45.0f)
		m_zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
