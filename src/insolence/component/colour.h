#ifndef COMPONENT_COLOUR_H
#define COMPONENT_COLOUR_H

#include "../insolence_dll.h"

#include "component.h"

struct INSOLENCE_API Colour : public Component
{
	glm::vec4 colour;
};

#endif
