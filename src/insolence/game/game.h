#ifndef GAME_H
#define GAME_H

#include "../insolence_dll.h"

#include <glm/vec3.hpp>

struct BaseGameLoop;
struct Entity;
struct EntityManager;

struct INSOLENCE_API Game {
private:
	Game() = default;
	~Game() = default;

public:
	static Game* Get() {
		static Game game;
		return &game;
	}

	struct {
		uint32_t window_width    = 640;
		uint32_t window_height   = 480;
		std::string window_title = "Game";
		bool is_resizable        = true;
	} startup_properties;

	static Entity* CreateEntity();

	EntityManager *entity_manager = nullptr;
	Window *window = nullptr;
	BaseGameLoop *loop = nullptr;

	glm::vec3 clear_colour;
	void Run();
};

#endif
