#ifndef MESH_H
#define MESH_H

#include "../insolence_dll.h"
#include "../game/log.h"
#include "../render/graphics.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* TODO Issue #23: Load in normal maps. */
struct INSOLENCE_API Mesh {
private:
	Mesh();
	Mesh(float* data_in, int vert_count);

	static unsigned int assimp_flags;

public:
	~Mesh();

	const GLuint num_verts;
	const float *data;

	static const aiScene* GetAiScene(const char *path);

	static Mesh* LoadFile(const char *path)
	{
		const aiScene *scene = GetAiScene(path);

		if(scene == NULL)
		{
			log(Log::FATAL, "Mesh (%s) - File \'%s\' not found",
					__FUNCTION__, path);
			return NULL;
		}

		unsigned int vert_count = scene->mMeshes[0]->mNumVertices;
		int attrib_length = 8;
		float *mesh_data = new float[vert_count * 8];

		for(int i = 0; i < vert_count; ++i)
		{
			for(int j = 0; j < 3; ++j)
				mesh_data[(attrib_length * i) + j] =
					scene->mMeshes[0]->mVertices[i][j];
		}

		if(scene->mMeshes[0]->HasNormals() == true)
		{
			for(int i = 0; i < vert_count; ++i)
				for(int j = 0; j < 3; ++j)
					mesh_data[(attrib_length * i) + 3 + j] =
						scene->mMeshes[0]->mNormals[i][j];
		}
		else
			log(Log::WARN, "Mesh (%s) - %s has no normal data.",
					__FUNCTION__, path);

		if(scene->mMeshes[0]->HasTextureCoords(0) > 0)
		{
			for(int i = 0; i < vert_count; ++i)
			{
				for(int j = 0; j < 2; ++j)
					mesh_data[(attrib_length * i) + 6 + j] =
						scene->mMeshes[0]->mTextureCoords[0][i][j];
			}
		}
		else
			log(Log::WARN, "Mesh (%s) - %s has no tex coord data.",
					__FUNCTION__, path);

		aiReleaseImport(scene);

		log(Log::INFO, "Mesh (%s) - Loaded %s successfully.",
				__FUNCTION__, path);

		return new Mesh(mesh_data, vert_count);
	}
};

#endif
