#ifndef SYSTEM_MESH_RENDERABLE_H
#define SYSTEM_MESH_RENDERABLE_H

#include "../insolence_dll.h"

#include "../component/mesh_renderable.h"

struct RenderManager3D;
template<>
struct INSOLENCE_API System<MeshRenderable> : SystemBase<MeshRenderable> {
private:
	RenderManager3D* renderer;

public:
	System<MeshRenderable>() {
		renderer = Game::Get()->renderers.mesh;
	}

	void Manage(const GameTime&);
};

#endif
