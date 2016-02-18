#ifndef RENDER_MANAGER_2D
#define RENDER_MANAGER_2D

#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "render_manager.h"

/* TODO Issue #13: Logging. */
struct RenderManager2D : public RenderManager
{
	typedef std::map<Texture*, std::vector<float> > data_def;

	GLuint vao;
	GLuint vbo_verts;
	GLuint vbo_data;

	GLint attrib_verts;
	GLint attrib_uv;
	GLint attrib_models[4];
	GLint attrib_colour;
	GLint attrib_model_uv;

	std::map<Texture*, std::vector<float> > data;
	std::map<Texture*, float> count;

	RenderManager2D();
	~RenderManager2D();

	/**
	 * Virtual from RenderManager - deals with flushing the draw.
	 */
	void Manage();

	/**
	 * Queue up a sprite to be drawn.
	 *
	 * \param tex		Pointer to texture.
	 * \param model		Model matrix of sprite.
	 * \param colour	colour of sprite.
	 */
	void Add(Texture *tex, const glm::mat4 &model, const glm::vec4& colour);

	/**
	 * Queue up a sprite to be drawn.
	 *
	 * \param tex		Pointer to texture.
	 * \param model		Model matrix of sprite.
	 * \param colour	Colour of sprite.
	 * \param rect		Sprite rectangle.
	 */
	void Add(Texture *tex, const glm::mat4 &model, const glm::vec4& colour,
			const glm::vec4& rect);
};

#endif
