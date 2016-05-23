#include "window.h"

std::unordered_map<GLFWwindow*, Window*> Window::windows;

Window::Window()
	:
		should_close(false)
{
}

void Window::SetWindowTitle(const char *title)
{
	glfwSetWindowTitle(glfw_window, title);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(glfw_window);
}
