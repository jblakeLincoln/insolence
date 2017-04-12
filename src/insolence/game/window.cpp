#include "window.h"

Window *Window::active_window = NULL;
bool Window::should_quit = false;
std::string Window::text_input;

Window::Window()
	:
		should_close(false)
{
}

void Window::SetTitle(const char *format, ...)
{
	char *title;
	va_list args;

	va_start(args, format);
	if(vasprintf(&title, format, args) < 0)
		log(Log::ERR, "%s: vasprintf error", __FUNCTION__);
	va_end(args);

	SDL_SetWindowTitle(sdl_window, title);
	free(title);
}

void Window::SwapBuffers()
{
	SDL_GL_SwapWindow(sdl_window);
}
