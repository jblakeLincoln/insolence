#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "../insolence_dll.h"

#include "../render/graphics.h"
#include "gametime.h"

#ifdef INSOLENCE_WEBGL
#include <emscripten.h>
#endif

struct Entity;
struct EntityManager;
struct Window;
struct INSOLENCE_API BaseGameLoop {
private:
	const double FRAME_TIME = 1000.f / 60.f;

	GameTime gametime;

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

#ifdef INSOLENCE_WEBGL
	static void RunWebGL(void *gw);
#endif

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
	virtual void Draw(const GameTime &gametime) {}

	/**
	 * Called once, when a call is made to close the window and exit. Use
	 * this to clean up any memory that may have been allocated.
	 */
	virtual void Unload()=0;

public:
	BaseGameLoop() {}
	virtual ~BaseGameLoop() {}

	/**
	 * Begins the game loop.
	 */
	void Start();
};

#endif
