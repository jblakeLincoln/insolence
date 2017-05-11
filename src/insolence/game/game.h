#ifndef GAME_H
#define GAME_H

#include "../insolence_dll.h"

#include <glm/vec3.hpp>
#include "resource_manager.h"

struct BaseGameLoop;
struct Console;
struct Entity;
struct EntityManager;

struct INSOLENCE_API Game {
private:
	Game() = default;
	~Game() = default;

	void CreateShaderPrograms();

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

	struct {
		ResourceManager<ShaderProgram> shader_programs;
	} resources;

	static Entity* CreateEntity();

	Console *console = nullptr;
	EntityManager *entity_manager = nullptr;
	Window *window = nullptr;
	BaseGameLoop *loop = nullptr;

	glm::vec3 clear_colour;
	void Run();
};

namespace EngineResources {
	enum ShaderProgram {
		DEFAULT_2D,
		DEFAULT_3D,
		DEFAULT_TEXT,
		PARTICLES
	};
};

#endif
