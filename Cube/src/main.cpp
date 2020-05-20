

#include <iostream>
#include <vector>
#include <math.h>

#include "gl3w/GL/gl3w.h"
#include "glfw/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_handling/shader_set_up.h"
#include "shader_handling/shader_modify.h"
#include "camera/camera.h"
#include "window/window.h"
#include "image_loading/stb_image.h"
#include "cube_mesh.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float deltaTime);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
	// ----- Setup window
	auto window = Window::getWindow(800, 600, "Showcase: Cube");
	if(not window)
	{
		fprintf(stderr, "Failed to get window\n");
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
	auto program = shader::set_up::linkProgramFromShaders(shaders, true);

	// ----- Setup buffers for data
	const auto& cube_vertices = CubeMesh::vertices;
	const auto& cube_texture_coords = CubeMesh::texture_coordinates;

	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	enum bufferPositions
	{
		VERTEX = 0,
		TEXTURE
	};

	std::vector<GLuint> buffers(2);
	glCreateBuffers(buffers.size(), &buffers.front());

	glBindBuffer(GL_ARRAY_BUFFER, buffers[bufferPositions::VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * cube_vertices.size(), &cube_vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[bufferPositions::TEXTURE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * cube_texture_coords.size(), &cube_texture_coords.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	// ----- Setup texturing
	glActiveTexture(GL_TEXTURE0);

	GLuint sampler_state;
	glCreateSamplers(1, &sampler_state);
	glSamplerParameteri(sampler_state, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(sampler_state, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(sampler_state, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler_state, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	GLuint texture;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int texture_width;
	int texture_height;
	int texture_nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	auto* data = stbi_load("../_Assets/textures/wooden_plank.jpg", &texture_width, &texture_height, &texture_nrChannels, 4);
	if(data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glUseProgram(program);
	shader::modify::setInt(program, "texture1", GL_TEXTURE0);

	// ----- Enabe/disable openGL functions
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

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

		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0);
		// Bind the sampler to the texture at GL_TEXTURE0 unit
		glBindSampler(0, sampler_state);

		processInput(window, deltaTime);

		glUseProgram(program);
		
		int width;
		int height;
		glfwGetWindowSize(window, &width, &height);

		height = (0 != height) ? height : 1;
		auto projection = glm::perspective(glm::radians(camera.m_zoom), (float)width / height, 0.1f, 100.0f);
		shader::modify::setMat4(program, "projection_matrix", projection);

		auto view = camera.GetViewMatrix();
		shader::modify::setMat4(program, "view_matrix", view);

		auto model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		shader::modify::setMat4(program, "model_matrix", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// ----- Cleanup
	glDeleteBuffers(buffers.size(), &buffers.front());
	glDeleteVertexArrays(1, &vao);
	glDeleteSamplers(1, &sampler_state);
	glDeleteTextures(1, &texture);
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
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