#include "mesh_renderable_system.h"

MeshRenderableSystem::MeshRenderableSystem(RenderManager3D* r)
:	renderer(r)
{}

void MeshRenderableSystem::Manage(const GameTime& gametime)
{
	for(it = components.begin(); it!= components.end(); ++it)
	{
		Movement *m = it->first->Get<Movement>();

		renderer->Add(it->second.mesh, it->second.material,
				it->second.colour, m->GetModelMatrix());
	}
}
