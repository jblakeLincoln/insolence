#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "entity.h"
#include "render_manager_2d.h"
#include "texture.h"

struct Entity;

struct Renderable
{
private:
	Renderable() {}
public:
	Entity *entity;

	Renderable(Entity *entity);
};

#endif
