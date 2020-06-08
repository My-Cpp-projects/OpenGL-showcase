



#ifndef WINDOW_H
#define WINDOW_H

#include "GL/gl3w.h"
#include "glfw/glfw3.h"
#include <string>
#include <memory>

class Window
{
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

public:
	~Window();
	static GLFWwindow* getWindow(int width, int height, std::string title);

private:
	static GLFWwindow* m_window;
};

#endif // !WINDOW_H

