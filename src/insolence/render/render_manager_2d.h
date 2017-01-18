#ifndef RENDER_MANAGER_2D
#define RENDER_MANAGER_2D

#include "../insolence_dll.h"

#include <map>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "render_manager.h"
#include "../game/font.h"

struct Font;
struct Texture;
struct INSOLENCE_API RenderManager2D : public RenderManager
{
	struct Data {
		glm::vec4 m1;
		glm::vec4 m2;
		glm::vec4 m3;
		glm::vec4 m4;
		glm::vec4 colour;
		glm::vec4 rect;
	};

private:
	/*
	 * We have a hash map of Texture/Data, for batching per texture,
	 * contained within an ordered map which is sorted by render order.
	 */
	typedef std::unordered_map<Texture*, std::vector<Data>> map_data_t;
	typedef std::map<int, map_data_t> data_2d_t;
	data_2d_t data;

	GLuint vao;

	struct VBOs {
		GLuint model;
		GLuint data;
	} vbos;

	struct ModelAttribs {
		GLint verts;
		GLint uvs;
	} model_attribs;

	struct DataAttribs {
		GLint model[4];
		GLint colour;
		GLint rect;
	} data_attribs;

	void Setup(const char *vs_path, const char *fs_path);
	void ModelBufferSetup();
	void DataBufferSetup();

public:
	RenderManager2D();
	RenderManager2D(const char *vs, const char *fs);
	~RenderManager2D();

	void Add(Texture*,
			const glm::mat4&,
			const glm::vec4 &colour = glm::vec4(1.f),
			const glm::vec4 &rect   = glm::vec4(0.f, 0.f, 1.f, 1.f),
			int layer               = 0);

	void Flush(const GameTime &gametime);
};

#endif
