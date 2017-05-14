#include "mesh_renderable_system.h"

#include "../component/transform.h"
#include "../render/render_manager_3d.h"

MeshRenderableSystem::MeshRenderableSystem(RenderManager3D* r)
	:	renderer(r)
{
}

void MeshRenderableSystem::Manage(const GameTime& gametime)
{
	for(it = 0; it < components.size(); ++it)
	{
		Transform *t = entities[it]->Get<Transform>();
		MeshRenderable &m = components[it];

		renderer->Add(m.mesh, m.material,
				t->GetModelMatrix(), m.colour);
	}
}
