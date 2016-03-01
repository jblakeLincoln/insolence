#ifndef COMPONENT_MATERIAL_H
#define COMPONENT_MATERIAL_H

#include "component.h"
#include "texture.h"

struct Material : public Component {
	Texture *diffuse;
	Texture *normal;
};

#endif
