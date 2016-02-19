#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "window.h"
#include "gametime.h"

struct BaseGameWorld {
private:
	Window *window;
	GameTime gametime;
	bool is_running;

public:
	void BaseInitialise();
	void BaseUpdate();
	void BaseDraw();
	void BaseUnload();

protected:
	/**
	 * First function called. Only runs once. Use this to initialise
	 * anything that needs setting up.
	 */
	virtual void Initialise()=0;

	/**
	 * Called once per tick. Run game logic in here.
	 *
	 * \param gametime	Time tracking of the game, giving things such as total
	 * 					elapsed time and time between frames.
	 */
	virtual void Update(const GameTime& gametime)=0;

	/**
	 * Called once per tick by Update. Use this to separate out your game
	 * logic from rendering.
	 */
	virtual void Draw()=0;

	/**
	 * Called once, when a call is made to close the window and exit. Use
	 * this to clean up any memory that may have been allocated.
	 */
	virtual void Unload()=0;

public:
	/**
	 * Begins the game loop.
	 */
	void Run();
};

#endif
