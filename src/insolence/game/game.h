#ifndef GAME_H
#define GAME_H

#include "../insolence_dll.h"

#include <glm/vec3.hpp>
#include "resource_manager.h"

struct BaseGameLoop;
struct Console;
struct Entity;
struct EntityManager;

struct Console;
struct Entity;
struct EntityManager;
struct RenderManager2D;
struct RenderManager3D;
struct RenderManagerSimpleParticles;
struct Window;
struct INSOLENCE_API Game {
private:
	Game() = default;
	~Game() = default;

	void CreateShaderPrograms();
	void CreateRenderers();

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

	struct {
		RenderManager2D *sprite;
		RenderManager3D *mesh;
		RenderManager2D *text;
		RenderManagerSimpleParticles *particles;
	} renderers;

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
