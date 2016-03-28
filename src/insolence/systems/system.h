#ifndef TEST_SYSTEM_H
#define TEST_SYSTEM_H

#include "../component/component.h"

struct System {
	virtual void Manage()=0;
};

#endif
