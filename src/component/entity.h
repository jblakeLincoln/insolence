#ifndef ENTITY_H
#define ENTITY_H

struct Entity
{
	Entity();
	~Entity() {}

	virtual void Update()=0;
	virtual void Draw()=0;
};

#endif
