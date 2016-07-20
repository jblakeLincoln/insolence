#ifndef SIMPLE_PARTICLE_RENDERABLE_H
#define SIMPLE_PARTICLE_RENDERABLE_H

#include "component.h"
#include "transform.h"
#include "../render/render_manager_simple_particles.h"
#include "../component/simple_particle_renderable.h"

struct SimpleParticleParams {
	uint32_t max               = 0;
	int death_timeout          = -1;
	glm::vec2 speed_range      = glm::vec2(0.1f, 0.2f);
	glm::vec2 scale_range      = glm::vec2(1.f, 1.f);
	glm::vec2 pos_range        = glm::vec2(-1.f, 1.f);
	glm::vec2 colour_r_range   = glm::vec2(0.f, 1.f);
	glm::vec2 colour_g_range   = glm::vec2(0.f, 1.f);
	glm::vec2 colour_b_range   = glm::vec2(0.f, 1.f);
	float fade_rate            = 0;
	int death_rate             = 1;
};

struct SimpleParticleRenderableSystem;
struct SimpleParticleRenderable : Component
{
friend SimpleParticleRenderableSystem;
protected:
	bool uninitialised = true;
	bool alive = true;

	std::vector<RenderManagerSimpleParticles::Layout> particles;
	std::vector<glm::vec2> vecs;
	TimeSpan life_timer;

	SimpleParticleParams params;

public:
	bool IsAlive() { return alive; }

	SimpleParticleRenderable(SimpleParticleParams params)
	:
		params(params)
	{}
};

#endif
