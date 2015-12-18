#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "entity.h"
#include "render_manager_2d.h"
#include "texture.h"

struct Entity;

static RenderManager2D *default_2d_renderer;
struct Renderable
{
private:
	Renderable() {}
public:
	Entity *entity;

	Renderable(Entity *entity);

	/**
	 * Virtual draw function to use renderable however is necessary.
	 */
	virtual void Draw()=0;

	static RenderManager2D* GetDefault2DRenderer()
	{
		if(default_2d_renderer == NULL)
			default_2d_renderer = new RenderManager2D();

		return default_2d_renderer;
	}
};

#endif
