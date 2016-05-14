#ifndef COMPONENT_MATERIAL_H
#define COMPONENT_MATERIAL_H

#include "../insolence_dll.h"

#include "texture.h"

struct INSOLENCE_API Material {
	Texture *diffuse;
	Texture *normal;
};

#endif
