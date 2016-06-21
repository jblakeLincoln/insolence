#ifndef WINDOW_H
#define WINDOW_H

#include "../insolence_dll.h"


#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../game/log.h"
#include "input.h"
#include "keys.h"

struct INSOLENCE_API Window
{
private:
	static std::unordered_map<GLFWwindow*, Window*> windows;

	static const GLuint GL_MAJOR = 3;
	static const GLuint GL_MINOR = 1;

	static void WindowFramebufferSizeCallback(GLFWwindow *w, int width,
			int height)
	{
		if(windows.find(w) == windows.end())
			return;

		windows[w]->framebuffer_width = width;
		windows[w]->framebuffer_height = height;
	}

	static void WindowResizeCallback(GLFWwindow *w, int width, int height)
	{
		if(windows.find(w) == windows.end())
			return;

		windows[w]->width = width;
		windows[w]->height = height;
	}

	Window();
	~Window() {}

	GLFWwindow *glfw_window;

	uint32_t width;
	uint32_t height;
	int framebuffer_width;
	int framebuffer_height;

	bool should_close;

public:
	uint32_t GetWidth() { return width; }
	uint32_t GetHeight() { return height; }
	int GetFramebufferWidth() { return framebuffer_width; }
	int GetFramebufferHeight() { return framebuffer_height; }

	bool ShouldClose() { return glfwWindowShouldClose(glfw_window); }
	void SetShouldClose(bool c) { glfwSetWindowShouldClose(glfw_window, c); }

	void SetTitle(const char *format, ...) {
		char *title;
		va_list args;

		va_start(args, format);
		if(vasprintf(&title, format, args) < 0)
			log(Log::ERR, "%s: vasprintf error", __FUNCTION__);
		va_end(args);

		glfwSetWindowTitle(glfw_window, title);
		free(title);
	}

	void SwapBuffers();

	/**
	 * Create an OpenGL 3.1 window with OpenGL context.
	 *
	 * \param width		Window width
	 * \param height	Window height
	 * \return			Created window
	 */
	static Window* Create(GLuint width=640, GLuint height=480,
			const char* title="Insolence Window", bool resizeable=false)
	{
		Window *out = new Window();

		/* TODO Issue #2: Log success/failure. */
		if(glfwInit() != GL_TRUE)
		{
			log(Log::FATAL, "Window (%s) - Failed to initialise GLFW",
					__FUNCTION__);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MINOR);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		/* TODO Issue #3: AA not dependent on GLFW. */
		glfwWindowHint(GLFW_SAMPLES, 4);

		if(resizeable == false)
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		out->glfw_window = glfwCreateWindow(width, height, title,
				NULL, NULL);

		out->width = width;
		out->height = height;

		glfwMakeContextCurrent(out->glfw_window);

		if(resizeable == true)
			glfwSetWindowSizeCallback(out->glfw_window, WindowResizeCallback);

		glfwSwapInterval(1);

		glewExperimental = GL_TRUE;

		if(glewInit() != GLEW_OK)
		{
			log(Log::FATAL, "Window (%s) - Failed to initialise GLEW.",
					__FUNCTION__);
		}

		Input::AttachWindowToKeyboard(out->glfw_window);
		Input::SetMouseWindow(out->glfw_window);

		windows[out->glfw_window] = out;
		glfwGetFramebufferSize(out->glfw_window, &out->framebuffer_width,
				&out->framebuffer_height);

		glfwSetFramebufferSizeCallback(out->glfw_window,
				WindowFramebufferSizeCallback);

		log(Log::INFO, "Window (%s) - Successful window creation",
				__FUNCTION__);

		return out;
	}

	/**
	 * Destroy a window and clean up the memory.
	 *
	 * \param Window to be destroyed.
	 */
	static void Destroy(Window *w)
	{
		glfwDestroyWindow(w->glfw_window);

		delete w;
		w = NULL;
	}

	/**
	 * Process all window callbacks and update input.
	 */
	static void PollEvents()
	{
		glfwPollEvents();
		Input::Update();
		//Mouse::Update();
	}
};

#endif
