#ifndef COMPONENT_H
#define COMPONENT_H

#include "../insolence_dll.h"

#include <cstdint>
#include <cstddef>

#include <typeinfo>
#include <typeindex>

struct INSOLENCE_API Component {
	virtual ~Component() {}
};

#endif
