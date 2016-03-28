#ifndef COMPONENT_H
#define COMPONENT_H

struct Component {
public:
	uint32_t owner;

	virtual void Create() {}

	Component()
	{
		owner = -1;
		Create();
	}

	void SetOwner(uint32_t id)
	{
		owner = id;
	}
};

#endif
