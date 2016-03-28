#ifndef COMPONENT_COLOUR_H
#define COMPONENT_COLOUR_H

#include "component.h"

struct Colour : public Component
{
	glm::vec4 colour;
};

#endif
