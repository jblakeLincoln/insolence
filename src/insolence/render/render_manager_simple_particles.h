#ifndef RENDER_MANAGER_SIMPLE_PARTICLES
#define RENDER_MANAGER_SIMPLE_PARTICLES

#include "../insolence_dll.h"

#include <unordered_map>
#include "render_manager.h"

struct INSOLENCE_API RenderManagerSimpleParticles : public RenderManager
{
	struct Layout {
		glm::vec2 pos;
		glm::vec4 colour;
		float scale;
	};

	GLuint vao;
	GLuint vbo_verts;
	GLuint vbo_data;

	GLint attrib_verts;
	GLint attrib_pos;
	GLint attrib_colour;
	GLint attrib_scale;

	std::unordered_map<float*, uint32_t> data;

	RenderManagerSimpleParticles();
	~RenderManagerSimpleParticles();

	void Add(float *data, int count);
	void Flush();
};

#endif
