#ifndef RENDER_MANAGER_3D
#define RENDER_MANAGER_3D

#include "../insolence_dll.h"

#include "graphics.h"
#include "render_manager.h"

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Material;
struct Mesh;

/* TODO Issue #21: Logging. */
struct INSOLENCE_API RenderManager3D : public RenderManager
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
	typedef std::map<Material*, std::vector<float> > tex_def;

	DirectionalLight dir_light;

	GLint attrib_models[4];
	GLint attrib_colour;

	GLint uni_view_pos;
	GLint uni_direction;
	GLint uni_diffuse;
	GLint uni_ambient;
	GLint uni_specular;

	GLint is_normal_map;

	glm::vec3 view_pos;

	std::map<Mesh*, mesh_data_buffers> mesh_buffers;
	std::map<Mesh*, std::map<Material*, std::vector<float> > > mesh_data;

public:
	RenderManager3D();
	~RenderManager3D();

	/**
	 * Virtual from RenderManager - deals with flushing the draw.
	 */
	void Flush(const GameTime &gametime);

	/**
	 * Queue up a mesh to be drawn.
	 */
	void Add(Mesh*, Material*, const glm::vec4& colour,
			const glm::mat4& model);

	/**
	 * Set camera position for accurate lighting.
	 *
	 * \param vp	Vec3 position of camera in world space.
	 */
	void SetViewPosition(const glm::vec3& vp);
};

#endif
