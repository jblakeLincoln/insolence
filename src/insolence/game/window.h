#ifndef WINDOW_H
#define WINDOW_H

#include "../insolence_dll.h"

#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "../game/log.h"
#include "input.h"
#include "keys.h"

struct INSOLENCE_API Window
{
private:
	static const GLuint GL_MAJOR = 3;
	static const GLuint GL_MINOR = 1;
	static Window *active_window;

	Window();
	~Window() {}

	SDL_Window *sdl_window;
	SDL_GLContext sdl_context;

	uint32_t width;
	uint32_t height;
	int framebuffer_width;
	int framebuffer_height;

	bool should_close;
	static bool should_quit;

public:
	uint32_t GetWidth() { return width; }
	uint32_t GetHeight() { return height; }
	int GetFramebufferWidth() { return framebuffer_width; }
	int GetFramebufferHeight() { return framebuffer_height; }

	bool ShouldClose() { return should_close || should_quit; }
	void SetShouldClose(bool c) { should_close = c; }
	static void SetShouldQuit(bool c) { should_quit = c; }

	void SetTitle(const char *format, ...);

	void SwapBuffers();

	/**
	 * Create an OpenGL 3.1 window with OpenGL context.
	 *
	 * \param width		Window width
	 * \param height	Window height
	 * \return			Created window
	 */
	static Window* Create(GLuint width=640, GLuint height=480,
			const char* title="Insolence Window", bool resizable=false)
	{
		if(Window::active_window != NULL)
		{
			log(Log::WARN, "Window (%s) - Window already present.",
					__FUNCTION__);
			return NULL;
		}

		Window *out = new Window();
		uint32_t window_flags = SDL_WINDOW_OPENGL;

		if(resizable == true)
			window_flags |= SDL_WINDOW_RESIZABLE;

		if(SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			log(Log::FATAL, "Window (%s) - Failed to initialise video",
					__FUNCTION__);
		}

		if(SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
			log(Log::FATAL, "Window (%s) - Failed to initialise GameController",
					__FUNCTION__);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
				SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_MAJOR);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_MINOR);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

		out->sdl_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED, width, height, window_flags);

		out->should_close = false;
		out->should_quit = false;

		if(out->sdl_window == NULL)
			log(Log::FATAL, "Window (%s) - Failed to create window",
					__FUNCTION__);

		out->sdl_context = SDL_GL_CreateContext(out->sdl_window);

		if(out->sdl_context == NULL)
			log(Log::FATAL, "Window (%s) - Failed to create GL context");

		glewExperimental = GL_TRUE;

		if(glewInit() != GLEW_OK)
		{
			log(Log::FATAL, "Window (%s) - Failed to initialise GLEW",
					__FUNCTION__);
		}

		out->HandleWindowResize();
		Window::active_window = out;

		PollEvents();

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
		if(w == NULL)
			return;

		SDL_GL_DeleteContext(w->sdl_context);
		SDL_DestroyWindow(w->sdl_window);
		SDL_Quit();

		delete w;
		w = NULL;
	}

private:
	void HandleWindowResize()
	{
		int w;
		int h;

		SDL_GetWindowSize(sdl_window, &w, &h);
		width = w;
		height = h;
		SDL_GL_GetDrawableSize(sdl_window, &w, &h);
		framebuffer_width = w;
		framebuffer_height = h;
	}

	static void HandleWindowEvents(SDL_Event *event)
	{
		if(active_window == NULL)
			return;

		switch(event->window.event)
		{
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				active_window->HandleWindowResize();
				break;
		}
	}

public:
	/**
	 * Process all window callbacks and update input.
	 */
	static void PollEvents()
	{
		SDL_Event event;

		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					Window::SetShouldQuit(true);
					break;

				case SDL_CONTROLLERDEVICEADDED:
					Input::OpenJoystick(event.cdevice.which);
					break;

				case SDL_CONTROLLERDEVICEREMOVED:
					Input::CloseJoystick(event.cdevice.which);
					break;

				case SDL_WINDOWEVENT_RESIZED:
				case SDL_WINDOWEVENT:
					HandleWindowEvents(&event);
					break;

				case SDL_CONTROLLERBUTTONUP:
				case SDL_CONTROLLERBUTTONDOWN:
					Input::SetGamepadButton(event.cbutton.which,
							event.cbutton.button,
							event.cbutton.state == SDL_PRESSED);
					break;

				case SDL_JOYAXISMOTION:
					Input::SetGamepadAxis(event.caxis.which,
							event.caxis.axis,
							event.caxis.value);
					break;

				case SDL_KEYDOWN:
				case SDL_KEYUP:
					Input::SetKey(event.key.keysym.sym,
							event.type == SDL_KEYDOWN);
					break;

				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
					Input::SetMouseButton(event.button.button,
							event.button.state == SDL_PRESSED);
					break;
			}
		}

		Input::Update();
	}
};

#endif
