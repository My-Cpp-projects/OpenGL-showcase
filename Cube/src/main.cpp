



#include <iostream>
#include <vector>

#include "gl3w/GL/gl3w.h"
#include "glfw/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_handling/shader_set_up.h"
#include "shader_handling/shader_modify.h"
#include "camera/camera.h"
#include "window/window.h"
#include "model_handling/model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float deltaTime);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
	// ----- Setup window
	auto window = Window::getWindow(800, 600, "Showcase: Cube");
	if(not window)
	{
		std::cerr << "Failed to get window\n";
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// ----- Setup shaders
	auto vertexShader = shader::set_up::loadFromFile("src/shaders/cube.vert", GL_VERTEX_SHADER);
	auto fragmentShader = shader::set_up::loadFromFile("src/shaders/cube.frag", GL_FRAGMENT_SHADER);
	std::vector<GLuint> shaders = { vertexShader, fragmentShader };
	auto cubeProgram = shader::set_up::linkProgramFromShaders(shaders, true);

	auto sun_vertexShader = shader::set_up::loadFromFile("src/shaders/sun.vert", GL_VERTEX_SHADER);
	auto sun_fragmentShader = shader::set_up::loadFromFile("src/shaders/sun.frag", GL_FRAGMENT_SHADER);
	std::vector<GLuint> sun_shaders = { sun_vertexShader, sun_fragmentShader };
	auto sunProgram = shader::set_up::linkProgramFromShaders(sun_shaders, true);

	// ----- Enabe/disable openGL functions
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// ---- Load models
	model_handling::Model cube("../_Assets/models/cube/cube.obj");
	model_handling::Model sun("../_Assets/models/low_poly_sphere/low_poly_sphere.obj");

	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// ----- Main loop
	while(glfwWindowShouldClose(window) != GL_TRUE)
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.0f, 0.25f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window, deltaTime);

		int width;
		int height;
		glfwGetWindowSize(window, &width, &height);

		// draw cube
		glUseProgram(cubeProgram);
		height = (0 != height) ? height : 1;
		auto projectionMat = glm::perspective(glm::radians(camera.m_zoom), (float)width / height, 0.1f, 100.0f);
		shader::modify::setMat4(cubeProgram, "projection_matrix", projectionMat);

		auto viewMat = camera.GetViewMatrix();
		shader::modify::setMat4(cubeProgram, "view_matrix", viewMat);

		auto modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f)); 
		modelMat = glm::scale(modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
		shader::modify::setMat4(cubeProgram, "model_matrix", modelMat);

		cube.draw(cubeProgram);
		// -- end draw cube

		// draw sun
		glUseProgram(sunProgram);
		modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, glm::vec3(3.0f, 3.0f, -3.0f));
		modelMat = glm::scale(modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
		shader::modify::setMat4(sunProgram, "projection_matrix", projectionMat);
		shader::modify::setMat4(sunProgram, "view_matrix", viewMat);
		shader::modify::setMat4(sunProgram, "model_matrix", modelMat);
		sun.draw(sunProgram);
		// -- end draw sun

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(cubeProgram);
	glDeleteProgram(sunProgram);
}

void processInput(GLFWwindow* window, float deltaTime)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::FORWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::LEFT, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(Camera_Movement::RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static float lastX = 0.0f;
	static float lastY = 0.0f;
	static bool firstMouse = true;

	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	static auto cursorMode = GLFW_CURSOR_DISABLED;
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		switch(cursorMode)
		{
			case GLFW_CURSOR_DISABLED:
				cursorMode = GLFW_CURSOR_NORMAL;
				break;
			case GLFW_CURSOR_NORMAL:
				cursorMode = GLFW_CURSOR_DISABLED;
				break;
		}

		glfwSetInputMode(window, GLFW_CURSOR, cursorMode);
	}
}