#ifndef ENTITY_H
#define ENTITY_H

#include <typeinfo>
#include <vector>

#include "component.h"

static uint32_t id_count = 0;

struct Entity
{
	uint32_t id;

	std::vector<Component*> components;

	void Add(Component *c)
	{
		components.push_back(c);

		if(c->owner == -1)
			c->SetOwner(this->id);
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

	Entity() {
		id = id_count++;
	}
	~Entity()
	{
		for(int i = 0; i < components.size(); ++i)
		{
			if(components[i]->owner != id)
				continue;

			delete components[i];
			components[i] = 0;
		}
	}
};

#endif
