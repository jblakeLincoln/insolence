#include "bounding_box.h"

#include <glm/vec2.hpp>

BoundingBox::BoundingBox(float x, float y, float w, float h)
:	x(x),
	y(y),
	w(w),
	h(h)
{
}

bool BoundingBox::CheckCollision(BoundingBox *a, BoundingBox *b)
{
	glm::vec2 mina, minb, maxa, maxb;

	mina.x = a->x;
	maxa.x = a->x + a->w;
	minb.x = b->x;
	maxb.x = b->x + b->w;

	mina.y = a->y;
	maxa.y = a->y + a->h;
	minb.y = b->y;
	maxb.y = b->y + b->h;


	for(int i = 0; i < 2; ++i)
		if(maxa[i] < minb[i] || mina[i] > maxb[i])
			return false;

	return true;
}
