#ifndef COMPONENT_BOUNDING_BOX_H
#define COMPONENT_BOUNDING_BOX_H

#include "../insolence_dll.h"

struct INSOLENCE_API BoundingBox {
	float x;
	float y;
	float w;
	float h;

	BoundingBox() {}
	BoundingBox(float x, float y, float w, float h);

	static bool CheckCollision(BoundingBox *a, BoundingBox *b);
};

#endif
