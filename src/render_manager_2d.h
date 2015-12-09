#ifndef RENDER_MANAGER_2D
#define RENDER_MANAGER_2D

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "render_manager.h"

/* TODO Issue #13: Logging. */
struct RenderManager2D : public RenderManager
{
	GLuint vao;
	GLuint vbo_verts;
	GLuint vbo_data;

	GLint attrib_verts;
	GLint attrib_uvs;
	GLint attrib_models[4];
	GLint attrib_colour;
	std::vector<float> data;
	int count;

	RenderManager2D();
	~RenderManager2D();

	/**
	 * Virtual from RenderManager - deals with flushing the draw.
	 */
	void Manage();

	/**
	 * Queue up a sprite to be drawn.
	 *
	 * \param model		Model matrix of sprite.
	 * \param colour	colour of sprite.
	 */
	void Add(const glm::mat4 &model, const glm::vec4& colour);
};

#endif
