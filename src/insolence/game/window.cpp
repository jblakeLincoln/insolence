#include "window.h"

std::unordered_map<GLFWwindow*, Window*> Window::windows;

Window::Window()
	:
		should_close(false)
{
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(glfw_window);
}
