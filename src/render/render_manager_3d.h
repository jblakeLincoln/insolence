#ifndef RENDER_MANAGER_3D
#define RENDER_MANAGER_3D

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include "../component/mesh.h"
#include "../component/texture.h"
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

	struct DirectionalLight {
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 ambient;
		glm::vec3 specular;
	};

	typedef std::map<Mesh*, mesh_data_buffers> mesh_def;
	typedef std::map<Texture*, std::vector<float> > tex_def;

	DirectionalLight dir_light;

	GLint attrib_models[4];
	GLint attrib_colour;

	GLuint uni_view_pos;
	GLuint uni_direction;
	GLuint uni_diffuse;
	GLuint uni_ambient;
	GLuint uni_specular;

	glm::vec3 view_pos;

	std::map<Mesh*, mesh_data_buffers> mesh_buffers;
	std::map<Mesh*, std::map<Texture*, std::vector<float> > > mesh_data;

public:
	RenderManager3D();
	~RenderManager3D();

	/**
	 * Queue up a mesh to be drawn.
	 */
	void Add(Mesh*, Texture*, const glm::vec4& colour, const glm::mat4& model);

	/**
	 * Set camera position for accurate lighting.
	 *
	 * \param vp	Vec3 position of camera in world space.
	 */
	void SetViewPosition(const glm::vec3& vp);

	/**
	 * Draw everything that has been added to the draw queue since the last
	 * call to Manage.
	 */
	void Manage();
};

#endif
