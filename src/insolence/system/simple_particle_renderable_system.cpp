#include "simple_particle_renderable_system.h"

SimpleParticleRenderableSystem::SimpleParticleRenderableSystem(
		RenderManagerSimpleParticles *r)
	:	renderer(r)
{
}

void SimpleParticleRenderableSystem::EndCreation(Entity *e,
		SimpleParticleRenderable *c)
{
	r_eng.seed(time(NULL));

	/* Always generate normalised velocity - speed can play with it. */
	std::uniform_real_distribution<float>
		dist_vec(-1.f, 1.f);

	std::uniform_real_distribution<float>
		dist_pos(c->params.pos_range.x, c->params.pos_range.y);
	std::uniform_real_distribution<float>
		dist_col_r(c->params.colour_r_range.x, c->params.colour_r_range.y);
	std::uniform_real_distribution<float>
		dist_col_g(c->params.colour_g_range.x, c->params.colour_g_range.y);
	std::uniform_real_distribution<float>
		dist_col_b(c->params.colour_b_range.x, c->params.colour_b_range.y);
	std::uniform_real_distribution<float>
		dist_speed(c->params.speed_range.x, c->params.speed_range.y);
	std::uniform_real_distribution<float>
		dist_scale(c->params.scale_range.x, c->params.scale_range.y);

	c->particles.resize(c->params.max);
	c->vecs.resize(c->params.max);

	for(int i = 0; i < c->params.max; ++i)
	{
		c->particles[i].pos.x = dist_pos(r_eng);
		c->particles[i].pos.y = dist_pos(r_eng);
		c->particles[i].scale = dist_scale(r_eng);
		c->particles[i].colour.r = dist_col_r(r_eng);
		c->particles[i].colour.g = dist_col_g(r_eng);
		c->particles[i].colour.b = dist_col_b(r_eng);
		c->particles[i].colour.a = 1.f;
		c->vecs[i] = glm::vec2(dist_vec(r_eng), dist_vec(r_eng)) *
			glm::vec2(dist_speed(r_eng), dist_speed(r_eng));
	}

	for(int i = 0; i < c->particles.size(); ++i)
		c->particles[i].pos += e->Get<Transform>()->GetPosXY();

	c->uninitialised = false;
}

void SimpleParticleRenderableSystem::Manage(const GameTime &gametime,
		Entity *e, SimpleParticleRenderable *c)
{
	if(c->alive == false)
		return;

	std::uniform_int_distribution<int> dist_vec(0, c->params.death_rate);
	c->life_timer += gametime.GetFrameTime();

	if(c->params.death_timeout > 0 &&
			c->life_timer.ElapsedMilliseconds() > c->params.death_timeout)
	{
		float fade = c->params.fade_rate == 0 ? 1.f : c->params.fade_rate *
			(gametime.GetFrameTime().ElapsedMilliseconds() / (1000 / 60.f));

		for(int i = 0; i < c->particles.size(); ++i)
		{
			c->particles[i].colour.a -= c->particles[i].colour.a < 1.f ||
				dist_vec(r_eng) != 0 ? fade : 0;
		}
	}

	if((c->params.death_timeout > 0 && c->life_timer.ElapsedMilliseconds() >
				c->params.death_timeout * 5.0) || c->particles.size() == 0)
	{
		c->alive = false;
		return;
	}

	for(int i = 0; i < c->particles.size(); ++i)
		c->particles[i].pos += c->vecs[i];

	renderer->Add((float*)&c->particles[0], c->particles.size());
}
