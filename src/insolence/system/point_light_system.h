#ifndef POINT_LIGHT_SYSTEM_H
#define POINT_LIGHT_SYSTEM_H

#include "../component/point_light.h"

struct RenderManager3D;
struct PointLightSystem : SystemBase<PointLight> {
	RenderManager3D *renderer;
	PointLightSystem();

	void Manage(const GameTime&, Entity&, PointLight&);
	void OnAdd(Entity&, PointLight&);
	void OnRemove(Entity&, PointLight&);
};

#endif
