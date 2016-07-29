#ifndef RENDER_MANAGER_2D
#define RENDER_MANAGER_2D

#include "../insolence_dll.h"

#include <map>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../game/font.h"
#include "render_manager.h"

struct Texture;
/* TODO Issue #13: Logging. */
struct INSOLENCE_API RenderManager2D : public RenderManager
{
	typedef std::unordered_map<Texture*, std::vector<float> > def_tex_data;
	typedef std::map<uint32_t, def_tex_data> def_layer_data;

	GLuint vao;
	GLuint vbo_verts;
	GLuint vbo_data;

	GLint attrib_verts;
	GLint attrib_uv;
	GLint attrib_models[4];
	GLint attrib_colour;
	GLint attrib_model_uv;

	std::map<uint32_t, std::unordered_map<Texture*, std::vector<float>>> data;
	std::map<uint32_t, std::unordered_map<Texture*, float> >count;

	RenderManager2D(const char *vs=NULL, const char *fs=NULL);
	~RenderManager2D();

	/**
	 * Virtual from RenderManager - deals with flushing the draw.
	 */
	void Flush();

	/**
	 * Queue up a sprite to be drawn.
	 *
	 * \param tex		Pointer to texture.
	 * \param model		Model matrix of sprite.
	 * \param colour	colour of sprite.
	 * \param layer		Placement in draw queue, 0 drawn last.
	 */
	void Add(Texture *tex, const glm::mat4 &model, const glm::vec4& colour,
			uint32_t layer);

	/**
	 * Queue up a sprite to be drawn.
	 *
	 * \param tex		Pointer to texture.
	 * \param model		Model matrix of sprite.
	 * \param colour	Colour of sprite.
	 * \param rect		Sprite rectangle.
	 * \param layer		Placement in draw queue, 0 drawn last.
	 *
	 */
	void Add(Texture *tex, const glm::mat4 &model, const glm::vec4& colour,
			const glm::vec4 &rect, uint32_t layer);

	/**
	 * Queue up text sprites to be drawn.
	 *
	 * \param f		Font to draw.
	 * \param text	Text to draw from ASCII range 32 to 128.
	 * \param pos	Position to start drawing from (considering alignment).
	 * \param col	Colour to draw.
	 * \param align	Alignment of font, affecting draw position.
	 * \param scale	Scaling of text, default 1.
	 */
	void AddText(Font* f, const char *text, const glm::vec2& pos,
			const glm::vec4& colour, TextAlignH::AlignH alignh,
			TextAlignV::AlignV alignv, glm::vec2 scale=glm::vec2());
};

#endif
