#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "component.h"
#include "../render/render_manager_3d.h"

struct PointLight : Component {
	RenderManager3D::PointLight *light;
};

#endif
