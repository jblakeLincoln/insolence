#ifndef MESH_H
#define MESH_H

#include "../insolence_dll.h"
#include "../game/log.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

/* TODO Issue #23: Load in normal maps. */
struct INSOLENCE_API Mesh {
private:
	Mesh();
	Mesh(float* data_in, int vert_count);

public:
	~Mesh();

	const GLuint num_verts;
	const float *data;

	static Mesh* LoadFile(const char *file)
	{
		const aiScene *scene = aiImportFile(file,
			aiProcess_CalcTangentSpace |
			//aiProcess_Triangulate |
			//aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		if(scene == NULL)
		{
			log(Log::FATAL, "Mesh (%s) - Failed to initialise aiImportFile.",
					__FUNCTION__);
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

		if(scene->mMeshes[0]->mNormals != NULL)
		{
			for(int i = 0; i < vert_count; ++i)
				for(int j = 0; j < 3; ++j)
					mesh_data[(attrib_length * i) + 3 + j] =
						scene->mMeshes[0]->mNormals[i][j];
		}
		else
			log(Log::WARN, "Mesh (%s) - %s has no normal data.",
					__FUNCTION__, file);

		if(scene->mMeshes[0]->mTextureCoords != NULL)
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
					__FUNCTION__, file);

		aiReleaseImport(scene);

		log(Log::INFO, "Mesh (%s) - Loaded %s successfully.",
				__FUNCTION__, file);

		return new Mesh(mesh_data, vert_count);
	}
};

#endif
