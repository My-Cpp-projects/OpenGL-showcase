

#include <iostream>
#include <vector>
#include <math.h>

#include "shader_handling/shader_set_up.h"
#include "shader_handling/shader_modify.h"
#include "camera/camera.h"
#include "window/window.h"

#include "gl3w/GL/gl3w.h"
#include "glfw/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float deltaTime);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
	constexpr unsigned int SCR_WIDTH = 1920;
	constexpr unsigned int SCR_HEIGHT = 1080;

	auto window = Window::getWindow(SCR_WIDTH, SCR_HEIGHT, "Showcase: Cube");
	if(not window)
	{
		fprintf(stderr, "Failed to get window\n");
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Setup shaders
	GLuint vertexShader = shader::set_up::loadFromFile("src/shaders/cube.vert", GL_VERTEX_SHADER);
	GLuint fragmentShader = shader::set_up::loadFromFile("src/shaders/cube.frag", GL_FRAGMENT_SHADER);
	std::vector<GLuint> shaders = { vertexShader, fragmentShader };
	GLuint program = shader::set_up::linkProgramFromShaders(shaders, true);

	// Vertices for cube
	static const std::vector<GLfloat> cube_vertices =
	{
		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f,
		 0.25f, -0.25f, -0.25f,
		 0.25f,  0.25f, -0.25f,
		 0.25f, -0.25f,  0.25f,
		 0.25f,  0.25f,  0.25f,
		-0.25f, -0.25f,  0.25f,
		-0.25f,  0.25f,  0.25f,
	};

	static const std::vector<GLushort> cube_indices =
	{
		0, 1, 2,
		2, 1, 3,
		2, 3, 4,
		4, 3, 5,
		4, 5, 6,
		6, 5, 7,
		6, 7, 0,
		0, 7, 1,
		6, 0, 2,
		2, 4, 6,
		7, 5, 3,
		7, 3, 1
	};

	// Create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create buffer for vertices
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	// Bind buffer to target
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	// Allocate memory for the buffer and pass the data to it
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * cube_vertices.size(), &cube_vertices.front(), GL_STATIC_DRAW);
	// Specify the way we arrange data in the vertex buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	// Create buffer for indices
	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	// Bind buffer to target
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	// Fill buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * cube_indices.size(), &cube_indices.front(), GL_STATIC_DRAW);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	bool running = true;

	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while(running)
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.0f, 0.25f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);
		processInput(window, deltaTime);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader::modify::setMat4(program, "projection_matrix", projection);

		glm::mat4 view = camera.GetViewMatrix();
		shader::modify::setMat4(program, "view_matrix", view);

		glm::mat4 model = glm::mat4(1.0f);
		shader::modify::setMat4(program, "model_matrix", model);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		running &= (glfwWindowShouldClose(window) != GL_TRUE);
	}

	glDeleteBuffers(1, &vertex_buffer);
	glDeleteVertexArrays(1, &vao);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void processInput(GLFWwindow* window, float deltaTime)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static float lastX = 0;
	static float lastY = 0;
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}