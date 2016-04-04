#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>
#include <cstddef>

#include <typeinfo>
#include <typeindex>

struct Component {
	virtual ~Component() {}
};

#endif
