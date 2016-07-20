#ifndef SIMPLE_PARTICLE_RENDERABLE_SYSTEM_H
#define SIMPLE_PARTICLE_RENDERABLE_SYSTEM_H

#include "../insolence_dll.h"

#include <random>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../component/simple_particle_renderable.h"
#include "system.h"

struct RenderManagerSimpleParticles;
struct SimpleParticleRenderableSystem : System<SimpleParticleRenderable>
{
private:
	std::default_random_engine r_eng;

	SimpleParticleRenderableSystem() {}
	RenderManagerSimpleParticles *renderer;
	void EndCreation(Entity *e, SimpleParticleRenderable *c);

public:
	SimpleParticleRenderableSystem(RenderManagerSimpleParticles*);
	void Manage(const GameTime &gametime, Entity *e, SimpleParticleRenderable*);
};
#endif
