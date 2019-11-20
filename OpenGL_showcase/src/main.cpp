



#include <iostream>
#include <vector>

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

	if(true)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	gl3wInit();

	GLuint vertexShader = shader::set_up::loadFromFile("assets/shaders/shader.vert", GL_VERTEX_SHADER);
	GLuint fragmentShader = shader::set_up::loadFromFile("assets/shaders/shader.frag", GL_FRAGMENT_SHADER);

	std::vector<GLuint> shaders = {vertexShader, fragmentShader};
	GLuint program = shader::set_up::linkFromShaders(shaders, true);

	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	bool running = true;

	do
	{
		GLfloat color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
		
		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();

		running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
		running &= (glfwWindowShouldClose(window) != GL_TRUE);
	} while(running);

	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);
	glfwDestroyWindow(window);
	glfwTerminate();
}
