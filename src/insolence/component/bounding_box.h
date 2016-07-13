#ifndef COMPONENT_BOUNDING_BOX_H
#define COMPONENT_BOUNDING_BOX_H

#include "../insolence_dll.h"

struct INSOLENCE_API BoundingBox : Component {
	float x;
	float y;
	float w;
	float h;

	BoundingBox() {}

	BoundingBox(float x, float y, float w, float h)
	:	x(x),
		y(y),
		w(w),
		h(h)
	{
	}

	static bool CheckCollision(BoundingBox *a, BoundingBox *b)
	{
		glm::vec2 mina, minb, maxa, maxb;

		mina.x = a->x - a->w / 2.f;
		maxa.x = a->x + a->w / 2.f;
		minb.x = b->x - b->w / 2.f;
		maxb.x = b->x + b->w / 2.f;

		mina.y = a->y - a->h / 2.f;
		maxa.y = a->y + a->h / 2.f;
		minb.y = b->y - b->h / 2.f;
		maxb.y = b->y + b->h / 2.f;


		for(int i = 0; i < 2; ++i)
			if(maxa[i] < minb[i] || mina[i] > maxb[i])
				return false;

		return true;
	}
};

#endif
