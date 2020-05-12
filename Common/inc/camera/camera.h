



#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <vector>

// Defines several possible options for camera movement.
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
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

	void ProcessMouseScroll(float yoffset);

	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};

#endif