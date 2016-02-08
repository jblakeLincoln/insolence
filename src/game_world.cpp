#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <cstdio>
#include <GL/glew.h>
#include "window.h"

struct BaseGameWorld {
private:
	Window *window;
	bool running;

	void BaseInitialise()
	{
		window = Window::CreateInsolenceWindow(500, 500, "Window Title");
		running = true;

		Initialise();
		BaseUpdate();
	}

	void BaseUpdate()
	{
		Update();
		BaseDraw();
	}

	void BaseDraw()
	{
		running = !window->ShouldClose();

		if(running == false)
		{
			Unload();
			return;
		}

		glClearColor(0.3f, 0.4f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		Draw();

		window->SwapBuffers();
		Window::PollEvents();

		BaseUpdate();
	}

	void BaseUnload()
	{
		Unload();

		Window::DestroyInsolenceWindow(window);
	}

protected:
	virtual void Initialise()=0;
	virtual void Update()=0;
	virtual void Draw()=0;
	virtual void Unload()=0;

public:
	void Run()
	{
		BaseInitialise();
	}

};


#endif
