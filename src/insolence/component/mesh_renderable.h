#ifndef COMPONENT_MESH_RENDERABLE_H
#define COMPONENT_MESH_RENDERABLE_H

#include "../insolence_dll.h"

#include "../game/material.h"
#include "../game/mesh.h"

#include <glm/vec4.hpp>

struct INSOLENCE_API MeshRenderable {
	Mesh *mesh;
	Material *material;
	glm::vec4 colour;

	MeshRenderable(Mesh *mesh=0, Material *material=Material::GetDefault(),
			const glm::vec4& colour=glm::vec4(1.f))
	:	mesh(mesh),
		material(material),
		colour(colour) {
	}
};

#endif
