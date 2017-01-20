#ifndef RENDER_MANAGER_3D
#define RENDER_MANAGER_3D

#include "../insolence_dll.h"

#include "render_manager.h"

#include <list>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

struct Material;
struct Mesh;
struct INSOLENCE_API RenderManager3D : public RenderManager {

	/*
	 * TODO: Dynamic number of point lights.
	 */
	static constexpr int MAX_POINT_LIGHTS = 32;

	struct Data {
		glm::mat4 model;
		glm::vec4 colour;
	};

	struct PointLight {
		glm::vec3 pos = glm::vec3(0.f, 0.f, 2.f);

		float constant  = 0.1f;
		float linear    = 0.09f;
		float quadratic = 5.8f;

		glm::vec3 ambient = glm::vec3(0.0f);
		glm::vec3 diffuse = glm::vec3(0.8f, 0.1f, 0.3f);
		glm::vec3 specular = glm::vec3(0.f);
	};

private:
	struct DirLight {
		glm::vec3 direction = glm::vec3(0.f, -0.3f, -0.4f);
		glm::vec3 diffuse   = glm::vec3(0.f);
		glm::vec3 ambient   = glm::vec3(0.0f);
		glm::vec3 specular  = glm::vec3(0.1f);
	} dir_light;

	struct PointLightUniforms {
		GLint pos;
		GLint constant;
		GLint linear;
		GLint quadratic;

		GLint diffuse;
		GLint ambient;
		GLint specular;
	} point_light_uniforms[RenderManager3D::MAX_POINT_LIGHTS];

	struct MeshBuffers {
		GLuint vao;
		GLuint mesh;
		GLuint model;
	};

	struct MeshAttribs {
		GLint verts;
		GLint norms;
		GLint uvs;
	} mesh_attribs;

	struct DataAttribs {
		GLint model[4];
		GLint colour;
	} data_attribs;

	struct DirLightUniforms {
		GLint direction;
		GLint diffuse;
		GLint ambient;
		GLint specular;
	} dir_light_uniforms;

	struct Uniforms {
		GLint view_pos;
		GLint tex_diffuse;
		GLint tex_normal;
		GLint is_normal_map;
	} uniforms;

	/*
	 * We have a lookup table for MeshBuffer struct per mesh, which contains a
	 * vertex buffers for the mesh and for the posting of data. We then store
	 * another lookup table of Materials to model data per mesh.
	 */
	typedef std::unordered_map<Mesh*, MeshBuffers> mesh_table_t;
	typedef std::unordered_map<Material*, std::vector<Data>> material_table_t;
	mesh_table_t meshes;
	std::unordered_map<Mesh*, material_table_t> data;

	std::list<PointLight*> point_lights;

	void CreateMeshBuffer(Mesh *m);
	void UpdatePointLight(const PointLight &, int index);

public:
	RenderManager3D();
	~RenderManager3D();
	void Add(Mesh *mesh, Material *material,
			const glm::mat4 &model, const glm::vec4 &colour);
	void Flush(const GameTime &gametime);

	PointLight* RequestPointLight();
	void FreePointLight(PointLight*);
};

#endif
