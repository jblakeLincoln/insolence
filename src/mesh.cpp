#include "mesh.h"

Mesh::Mesh()
	:	data(0),
		num_verts(0)
{}

Mesh::Mesh(float* data_in, int vert_count)
	:	data(data_in),
		num_verts(vert_count)
{}

Mesh::~Mesh()
{
	delete[] data;
}
