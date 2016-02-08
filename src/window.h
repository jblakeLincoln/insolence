#ifndef WINDOW_H
#define WINDOW_H

#include <cstdio>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "input.h"

struct Window
{
private:
	GLFWwindow *glfw_window;

	Window() {}
	~Window() {}

	GLuint width;
	GLuint height;
	bool should_close;

	/* TODO Issue #4: Add window options. */

public:
	bool ShouldClose() { return glfwWindowShouldClose(glfw_window); }
	void SetShouldClose(bool c) { glfwSetWindowShouldClose(glfw_window, c); }

	void SwapBuffers();

	/**
	 * Create an OpenGL 3.1 window with OpenGL context.
	 *
	 * \param width		Window width
	 * \param height	Window height
	 * \return			Created window
	 */
	static Window *CreateInsolenceWindow(GLuint width, GLuint height,
			const char *title)
	{
		Window *output = new Window();
		int gl_major = 3;
		int gl_minor = 1;

		/* TODO Issue #2: Log success/failure. */
		if(glfwInit() == GL_TRUE)
		{
			// Log success
		}
		else
		{
			// Log failure
			exit(-1);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		/* TODO Issue #3: AA not dependent on GLFW. */
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwSwapInterval(1);

		output->glfw_window = glfwCreateWindow(width, height, title,
				NULL, NULL);
		output->width = width;
		output->height = height;

		glfwMakeContextCurrent(output->glfw_window);

		glewExperimental = GL_TRUE;

		if(glewInit() != GLEW_OK)
		{
			// Log GLEW failure
			exit(-1);
		}

		Input::AttachWindowToKeyboard(output->glfw_window);
		Input::SetMouseWindow(output->glfw_window);

		// Log window creation success.
		return output;
	}

	/**
	 * Destroy a window and clean up the memory.
	 *
	 * \param Window to be destroyed.
	 */
	static void DestroyInsolenceWindow(Window *window)
	{
		glfwDestroyWindow(window->glfw_window);

		delete window;
		window = 0;
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
