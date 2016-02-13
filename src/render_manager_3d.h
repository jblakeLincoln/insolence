#ifndef RENDER_MANAGER_3D
#define RENDER_MANAGER_3D

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include "mesh.h"
#include "texture.h"
#include "render_manager.h"

/* TODO Issue #21: Logging. */
struct RenderManager3D : public RenderManager
{
private:
	struct mesh_data_buffers {
			GLuint vao;
			GLuint vbo_mesh;
			GLuint vbo_model;
	};

	typedef std::map<Mesh*, mesh_data_buffers> mesh_def;
	typedef std::map<Texture*, std::vector<float> > tex_def;

	GLint attrib_models[4];

	std::map<Mesh*, mesh_data_buffers> mesh_buffers;
	std::map<Mesh*, std::map<Texture*, std::vector<float> > > mesh_data;

public:
	RenderManager3D();
	~RenderManager3D();

	/**
	 * Queue up a mesh to be drawn.
	 */
	void Add(Mesh*, Texture*, const glm::mat4& model);

	/**
	 * Draw everything that has been added to the draw queue since the last
	 * call to Manage.
	 */
	void Manage();
};

#endif
