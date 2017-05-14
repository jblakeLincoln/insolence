#ifndef SIMPLE_PARTICLE_RENDERABLE_SYSTEM_H
#define SIMPLE_PARTICLE_RENDERABLE_SYSTEM_H

#include "../insolence_dll.h"

#include <random>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../component/simple_particle_renderable.h"
#include "../ecs/system.h"

struct RenderManagerSimpleParticles;
template<>
struct System<SimpleParticleRenderable> : SystemBase<SimpleParticleRenderable>
{
private:
	std::default_random_engine r_eng;
	RenderManagerSimpleParticles *renderer;


public:
	void OnAdd(Entity&, SimpleParticleRenderable&);
	System<SimpleParticleRenderable>() {
		renderer = Game::Get()->renderers.particles;
	}

	void Manage(const GameTime &gametime, Entity &e, SimpleParticleRenderable&);
};
#endif
