#ifndef COMPONENT_MESH_RENDERABLE_H
#define COMPONENT_MESH_RENDERABLE_H

struct MeshRenderable : Component {
	Mesh *mesh;
	Material *material;
	glm::vec4 colour;

	MeshRenderable() :
		mesh(0),
		material(0),
		colour(glm::vec4(1.f)) {}
};

#endif
