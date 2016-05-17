#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "../insolence_dll.h"
#include <chrono>
#include "entity.h"
#include "window.h"
#include "gametime.h"

struct Entity;
struct EntityManager;
struct INSOLENCE_API BaseGameWorld {
private:
	const double FRAME_TIME = 1000.f / 60.f;

	Window *window;
	GameTime gametime;
	bool is_running = false;

	/**
	 * Create a window and declare the game world as "running".
	 * Initialises GameTime and EntityManager.
	 */
	void BaseInitialise();

	/**
	 * Updates GameTime and calls the virtual Update.
	 * Then manages entities.
	 */
	void BaseUpdate();

	/**
	 * Checks if the window should close. If not, continues to clear the
	 * buffer and calls base draw, then flushes the EntityManager.
	 */
	void BaseDraw();

	/**
	 * Nothing before calling Unload, then destroys the window and
	 * entity manager.
	 */
	void BaseUnload();

public:
	virtual ~BaseGameWorld() {}

	Entity *CreateEntity();
	EntityManager *entity_manager;

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
	 *					elapsed time and time between frames.
	 */
	virtual void Update(const GameTime& gametime)=0;

	/**
	 * Called once per tick by Update. Use this to separate out your game
	 * logic from rendering.
	 */
	virtual void Draw() {}

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
