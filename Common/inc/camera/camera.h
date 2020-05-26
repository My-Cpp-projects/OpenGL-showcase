



#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <vector>

enum class Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// An camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		   float yaw = -90.0f,
		   float pitch = 0.0f);

	Camera(float posX, float posY, float posZ,
		   float upX, float upY, float upZ, 
		   float yaw, float pitch);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	void processKeyboard(Camera_Movement direction, float deltaTime);

	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

	void processMouseScroll(float yoffset);

	// Camera Attributes
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;
	// Euler Angles
	float m_yaw;
	float m_pitch;
	// Camera options
	float m_movementSpeed;
	float m_mouseSensitivity;
	float m_zoom;

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};

#endif