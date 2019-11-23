



#include <iostream>
#include <vector>
#include <math.h>

#include "gl3w/GL/gl3w.h"
#include "glfw/glfw3.h"
#include "shader_helpers/shader_set_up.h"

int main()
{
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "test", 0, nullptr);
	if(!window)
	{
		fprintf(stderr, "Failed to open window\n");
		return 1;
	}

	glfwMakeContextCurrent(window);

	gl3wInit();

	GLuint vertexShader = shader::set_up::loadFromFile("assets/shaders/vertex.glsl", GL_VERTEX_SHADER);
	GLuint tessellationControlShader = shader::set_up::loadFromFile("assets/shaders/tessellationControl.glsl", GL_TESS_CONTROL_SHADER);
	GLuint tessellationEvaluationShader = shader::set_up::loadFromFile("assets/shaders/tessellationEvaluation.glsl", GL_TESS_EVALUATION_SHADER);
	GLuint geometryShader = shader::set_up::loadFromFile("assets/shaders/geometry.glsl", GL_GEOMETRY_SHADER);
	GLuint fragmentShader = shader::set_up::loadFromFile("assets/shaders/fragment.glsl", GL_FRAGMENT_SHADER);

	std::vector<GLuint> shaders = { vertexShader, tessellationControlShader, tessellationEvaluationShader, geometryShader, fragmentShader };
	GLuint program = shader::set_up::linkProgramFromShaders(shaders, true);

	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	bool running = true;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLfloat backgroundColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	while(running)
	{
		glClearBufferfv(GL_COLOR, 0, backgroundColor);
		
		glUseProgram(program);
		double currentTime = glfwGetTime();
		GLfloat offset[] = {
			(float)sin(currentTime) * 0.5f,
			(float)cos(currentTime) * 0.6f,
			0.0f,
			0.0f};
		glVertexAttrib4fv(0, offset);

		glPointSize(5.0f);
		glDrawArrays(GL_PATCHES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();

		running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
		running &= (glfwWindowShouldClose(window) != GL_TRUE);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
	glfwDestroyWindow(window);
	glfwTerminate();
}
