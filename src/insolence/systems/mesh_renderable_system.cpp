#include "mesh_renderable_system.h"

MeshRenderableSystem::MeshRenderableSystem(RenderManager3D* r)
:	renderer(r)
{}

void MeshRenderableSystem::Manage(const GameTime& gametime)
{
	for(it = components.begin(); it != components.end(); ++it)
	{
		Transform *m = it->first->Get<Transform>();

		renderer->Add(it->second.mesh, it->second.material,
				it->second.colour, m->GetModelMatrix());
	}
}
