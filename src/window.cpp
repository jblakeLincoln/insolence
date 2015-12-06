#include "window.h"

void Window::SwapBuffers()
{
	glfwSwapBuffers(glfw_window);
}
