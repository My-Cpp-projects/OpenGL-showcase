



#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <memory>
#include "gl3w/GL/gl3w.h"
#include "glfw/glfw3.h"

class Window
{
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

public:
	static GLFWwindow* getWindow(int width, int height, std::string title);

private:
	static GLFWwindow* m_window;
};

#endif // !WINDOW_H

