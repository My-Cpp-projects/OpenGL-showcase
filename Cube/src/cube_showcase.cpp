



#include "cube_showcase.h"
#include "camera/camera.h"
#include "window/window.h"
#include "shader_handling/shader_program.h"
#include "texture_handling/texture_loader.h"
#include "drawables/model.h"
#include "drawables/skybox.h"

#include "gl3w/GL/gl3w.h"
#include "glfw/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float deltaTime);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

CubeShowcase::CubeShowcase(const std::string& sceneName)
	: ShowcaseBase(sceneName)
	, m_showcaseName(sceneName)
	, m_window(nullptr)
	, m_shaderPrograms(shaderPrograms::SHADER_COUNT)
	, m_buffers(buffer::BUFFER_COUNT)
	, m_sunPosition(-1)
	, m_pointLightPosition(-1)
	, m_lightColor(-1)
	, m_sunlightDir(-1)
{
	ShowcaseBase::init(this);
}

CubeShowcase::~CubeShowcase()
{
	cleanup();
}

void CubeShowcase::run()
{
	// ----- Enabe/disable openGL functions
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	auto deltaTime = 0.0f;
	auto lastFrame = 0.0f;
	auto& cubeShaderProgram = m_shaderPrograms[shaderPrograms::CUBE];
	auto& lightShaderProgram = m_shaderPrograms[shaderPrograms::LIGHT];
	auto& skyboxShaderProgram = m_shaderPrograms[shaderPrograms::SKYBOX];

	while(glfwWindowShouldClose(m_window) != GL_TRUE)
	{
		auto currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.0f, 0.25f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(m_window, deltaTime);

		int width;
		int height;
		glfwGetWindowSize(m_window, &width, &height);

		// update view and projection matrices in uniform buffer object
		height = (0 != height) ? height : 1;
		auto projectionMat = glm::perspective(glm::radians(camera.m_zoom), (float)width / height, 0.1f, 100.0f);
		auto viewMat = camera.GetViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, m_buffers[buffer::VP_MATRICES]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &viewMat[0][0]);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projectionMat[0][0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// draw cube
		cubeShaderProgram
			// material of object
			->setFloat("material.shininess", 32.0f)
			// camera position required for view_direction
			.setVec3("view_pos", camera.m_position)
			// point light		
			.setVec3("pointLight.position", m_pointLightPosition)
			.setVec3("pointLight.diffuse", { 0.8f, 0.8f, 0.8f })
			.setVec3("pointLight.ambient", { 0.05f, 0.05f, 0.05f })
			.setVec3("pointLight.specular", { 1.0f, 1.0f, 1.0f })
			.setFloat("pointLight.constant", 1.0f)
			.setFloat("pointLight.linear", 0.09)
			.setFloat("pointLight.quadratic", 0.032);

		// position in the scene
		auto modelMat = glm::mat4(1.0f);
		for(const auto& pos : m_cubePositions)
		{
			modelMat = glm::mat4(1.0f);
			modelMat = glm::translate(modelMat, pos);
			cubeShaderProgram->setMat4("model_matrix", modelMat);
			cubeShaderProgram->use();
			m_cube->draw(cubeShaderProgram->getProgramId());
		}
		// -- end draw cube

		// draw sun
		modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, m_sunPosition);
		modelMat = glm::scale(modelMat, glm::vec3(0.3, 0.3, 0.3));
		lightShaderProgram
			->setMat4("model_matrix", modelMat)
			.setVec3("light_color", m_lightColor);
		m_sun->draw(lightShaderProgram->getProgramId());
		// -- end draw sun

		// update point light position
		m_pointLightPosition.x = sin(glfwGetTime()) * 4.0f;
		m_pointLightPosition.y = sin(glfwGetTime() / 2) * cos(glfwGetTime() / 2) * 4.0f;
		m_pointLightPosition.z = cos(glfwGetTime()) * 4.0f;

		// draw point light
		modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, m_pointLightPosition);
		lightShaderProgram
			->setMat4("model_matrix", modelMat)
			.setVec3("light_color", m_lightColor);
		m_sun->draw(lightShaderProgram->getProgramId());
		// -- end draw point light

		// draw skybox
		m_skybox->draw(skyboxShaderProgram->getProgramId());
		// -- end draw skybox

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void CubeShowcase::setupWindow()
{
	m_window = Window::getWindow(1920, 1080, "Showcase: Cube");
	assert(m_window && "!!! Failed to get window !!!");

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetScrollCallback(m_window, scroll_callback);
}

void CubeShowcase::loadModels()
{
	m_cube = std::make_unique<drawables::Model>("../_Assets/models/cube/cube.obj");
	m_sun = std::make_unique<drawables::Model>("../_Assets/models/low_poly_sphere/low_poly_sphere.obj");

	std::vector<std::string> skyboxTextures
	{
			"../_Assets/textures/skyboxes/forest/right.jpg",
			"../_Assets/textures/skyboxes/forest/left.jpg",
			"../_Assets/textures/skyboxes/forest/top.jpg",
			"../_Assets/textures/skyboxes/forest/bottom.jpg",
			"../_Assets/textures/skyboxes/forest/front.jpg",
			"../_Assets/textures/skyboxes/forest/back.jpg"
	};
	m_skybox = std::make_unique<drawables::Skybox>(skyboxTextures);
}

void CubeShowcase::setupInitialPositions()
{
	m_cubePositions =
	{
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(5.0f,  0.0f,  -10.0f),
	};

	m_sunPosition = glm::vec3(10.0f, 10.0f, -10.0f);
	m_pointLightPosition = glm::vec3(4.0f);

	m_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	m_sunlightDir = glm::vec3(0.0f) - m_sunPosition; // from sun to origin
}

void CubeShowcase::loadShaders()
{
	auto cubeShader = std::make_unique<shader_handling::ShaderProgram>();
	cubeShader
		->attachShader("src/shaders/cube.vert", GL_VERTEX_SHADER)
		.attachShader("src/shaders/cube.frag", GL_FRAGMENT_SHADER)
		.linkProgram();
	m_shaderPrograms[shaderPrograms::CUBE] = std::move(cubeShader);

	auto lightSourceShader = std::make_unique<shader_handling::ShaderProgram>();
	lightSourceShader
		->attachShader("src/shaders/sun.vert", GL_VERTEX_SHADER)
		.attachShader("src/shaders/sun.frag", GL_FRAGMENT_SHADER)
		.linkProgram();
	m_shaderPrograms[shaderPrograms::LIGHT] = std::move(lightSourceShader);

	auto skyboxShader = std::make_unique<shader_handling::ShaderProgram>();
	skyboxShader
		->attachShader("src/shaders/skybox.vert", GL_VERTEX_SHADER)
		.attachShader("src/shaders/skybox.frag", GL_FRAGMENT_SHADER)
		.linkProgram();
	m_shaderPrograms[shaderPrograms::SKYBOX] = std::move(skyboxShader);
}

void CubeShowcase::setupShaders()
{
	glGenBuffers(m_buffers.size(), &m_buffers.front());
	const auto& cubeShaderProgram = m_shaderPrograms[shaderPrograms::CUBE]->getProgramId();
	const auto& lightShaderProgram = m_shaderPrograms[shaderPrograms::LIGHT]->getProgramId();
	const auto& skyboxShaderProgram = m_shaderPrograms[shaderPrograms::SKYBOX]->getProgramId();

	// set the uniform buffer object for VPMatrices
	auto cubeVPUniformBlockPos = glGetUniformBlockIndex(cubeShaderProgram, "VPMatrices");
	auto lightVPUniformBlockPos = glGetUniformBlockIndex(lightShaderProgram, "VPMatrices");
	auto skyboxVPUniformBlockPos = glGetUniformBlockIndex(skyboxShaderProgram, "VPMatrices");
	glUniformBlockBinding(cubeShaderProgram, cubeVPUniformBlockPos, buffer::VP_MATRICES);
	glUniformBlockBinding(lightShaderProgram, lightVPUniformBlockPos, buffer::VP_MATRICES);
	glUniformBlockBinding(skyboxShaderProgram, skyboxVPUniformBlockPos, buffer::VP_MATRICES);

	glBindBuffer(GL_UNIFORM_BUFFER, m_buffers[buffer::VP_MATRICES]);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_buffers[buffer::VP_MATRICES], 0, 2 * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// set the uniform buffer object for sunLight
	auto cubeSunLightUniformBlockPos = glGetUniformBlockIndex(cubeShaderProgram, "SunLight");
	glUniformBlockBinding(cubeShaderProgram, cubeSunLightUniformBlockPos, buffer::SUN_LIGHT);

	glBindBuffer(GL_UNIFORM_BUFFER, m_buffers[buffer::SUN_LIGHT]);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_buffers[buffer::SUN_LIGHT], 0, 4 * sizeof(glm::vec4));

	auto sunLight_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	auto sunLight_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	auto sunLight_specular = glm::vec3(1.0f, 1.0f, 1.0f);

	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::vec4), sizeof(glm::vec4), &m_sunlightDir);
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::vec4), sizeof(glm::vec4), &sunLight_ambient);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), &sunLight_diffuse);
	glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec4), &sunLight_specular);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void CubeShowcase::cleanup()
{
	glDeleteBuffers(m_buffers.size(), &m_buffers.front());
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

	if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if(glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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