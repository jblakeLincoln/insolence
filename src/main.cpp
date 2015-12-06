#include <stdio.h>

#include "window.h"

int main()
{
	Window *window = Window::CreateInsolenceWindow(500, 500, "Window Title");

	while(window->ShouldClose() == false)
	{

		window->SwapBuffers();
		Window::PollEvents();
	}

	Window::DestroyInsolenceWindow(window);

	return 0;
}
