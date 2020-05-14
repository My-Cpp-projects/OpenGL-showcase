

#include "window/window.h"

#include <iostream>

GLFWwindow* Window::m_window = nullptr;

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

GLFWwindow* Window::getWindow(int width, int height, std::string title)
{
	if(not m_window)
	{
		if(!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			return nullptr;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		m_window = glfwCreateWindow(width, height, title.c_str(), 0, nullptr);
		if(not m_window)
		{
			fprintf(stderr, "Failed to open window\n");
			return nullptr;
		}
		glfwMakeContextCurrent(m_window);

		// Has to be called after glfwMakeContextCurrent is called !
		if(gl3wInit())
		{
			fprintf(stderr, "failed to initialize OpenGL\n");
			return nullptr;
		}
	}

	return m_window;
}