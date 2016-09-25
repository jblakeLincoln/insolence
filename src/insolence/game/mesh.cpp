#include "mesh.h"

unsigned int Mesh::assimp_flags =
	aiProcess_CalcTangentSpace | aiProcess_SortByPType;

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

const aiScene* Mesh::GetAiScene(const char *path)
#if INSOLENCE_OPENGL_DESKTOP || INSOLENCE_WEBGL
{
	return aiImportFile(path, Mesh::assimp_flags);
}
#elif INSOLENCE_ANDROID
{
	FILE *file = fopen(path, "r");
	size_t file_size;
	char *buf;
	const aiScene *out;

	if(file == NULL)
		return NULL;

	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	rewind(file);

	buf = new char[file_size + 1];
	buf[file_size] = '\0';
	fread(buf, 1, file_size, file);
	fclose(file);

	out = aiImportFileFromMemory(buf, file_size, Mesh::assimp_flags, NULL);
	delete[] buf;

	return out;
}
#endif
