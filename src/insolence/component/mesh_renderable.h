#ifndef COMPONENT_MESH_RENDERABLE_H
#define COMPONENT_MESH_RENDERABLE_H

#include "component.h"
#include "../game/material.h"
#include "../game/mesh.h"

struct MeshRenderable : Component {
	Mesh *mesh;
	Material *material;
	glm::vec4 colour;

	MeshRenderable(Mesh *mesh=0, Material *material=0,
			const glm::vec4& colour=glm::vec4(1.f))
	:	mesh(mesh),
		material(material),
		colour(colour) {
	}
};

#endif
