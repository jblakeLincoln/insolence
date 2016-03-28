#ifndef COMPONENT_MATERIAL_H
#define COMPONENT_MATERIAL_H

#include "texture.h"

struct Material {
	Texture *diffuse;
	Texture *normal;
};

#endif
