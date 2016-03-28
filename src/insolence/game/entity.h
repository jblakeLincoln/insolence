#ifndef ENTITY_H
#define ENTITY_H

#include <typeinfo>
#include <vector>
#include <stdint.h>

#include "../component/component.h"
#include "../component/movement.h"

static uint32_t id_count = 0;

struct Entity
{
	uint32_t id;

	std::vector<Component*> components;

	void Add(Component *c)
	{
		components.push_back(c);
	}

	template <typename T>
	T* Get()
	{
		for(int i = 0; i < components.size(); ++i)
			if(typeid(T) == typeid(*components[i]))
				return (T*)components[i];

		return NULL;
	}

	template <typename T>
	bool Has()
	{
		for(int i = 0; i < components.size(); ++i)
			if(typeid(T) == typeid(*components[i]))
				return true;

		return false;
	}

	int NumComponents() { return components.size(); }

	Component* GetComponentByNum(int i) { return components[i]; }

	Entity() {
		id = id_count++;
		Add(new Movement());
	}
	~Entity()
	{
		for(int i = 0; i < components.size(); ++i)
		{
			//if(components[i]->owner != id)
			//	continue;

			delete components[i];
			components[i] = 0;
		}
	}
};

#endif
