

#include "camera/camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

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

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
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

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
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

void Camera::processMouseScroll(float yoffset)
{
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		m_zoom -= yoffset;
	if (m_zoom <= 1.0f)
		m_zoom = 1.0f;
	if (m_zoom >= 45.0f)
		m_zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);	
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
