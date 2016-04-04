#ifndef SYSTEM_MESH_RENDERABLE_H
#define SYSTEM_MESH_RENDERABLE_H

#include "../game/entity.h"
#include "../component/mesh_renderable.h"
#include "../render/render_manager_3d.h"

struct MeshRenderableSystem : System<MeshRenderable> {
private:
	RenderManager3D* renderer;

public:
	MeshRenderableSystem(RenderManager3D*);
	void Manage(const GameTime&);
};

#endif
