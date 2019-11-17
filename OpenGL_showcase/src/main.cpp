


#include "gl3w/GL/gl3w.h"
#include "glfw/glfw3.h"
#include <iostream>

GLuint compileShaders()
{
	const GLchar* vertex_shader_source = "\n"
		"#version 450 core \n"
		" \n"
		"const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0), \n"
		"									vec4(-0.25, -0.25, 0.5, 1.0), \n"
		"									vec4(0.25, 0.25, 0.5, 1.0)); \n"
		" \n"
		"void main(void)\n"
		"{\n"
		"	gl_Position = vertices[gl_VertexID]; \n"
		"}\n";

	const GLchar* fragment_shader_source = "\n"
		"#version 450 core\n"
		"out vec4 color; \n"
		"void main(void)\n"
		"{\n"
		"color = vec4(0.0, 0.8, 1.0, 1.0); \n"
		"}\n";

	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
	glCompileShader(fragment_shader);

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}

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
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "aaa", 0 ? glfwGetPrimaryMonitor() : NULL, NULL);
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

	// info.flags.stereo = (glfwGetWindowParam(GLFW_STEREO) ? 1 : 0);

	gl3wInit();

	GLuint program = compileShaders();
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
