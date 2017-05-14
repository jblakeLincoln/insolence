#ifndef SYSTEM_MESH_RENDERABLE_H
#define SYSTEM_MESH_RENDERABLE_H

#include "../insolence_dll.h"

#include "../component/mesh_renderable.h"

struct RenderManager3D;
struct INSOLENCE_API MeshRenderableSystem : System<MeshRenderable> {
private:
	RenderManager3D* renderer;

public:
	MeshRenderableSystem();
	void Manage(const GameTime&);
};

#endif
