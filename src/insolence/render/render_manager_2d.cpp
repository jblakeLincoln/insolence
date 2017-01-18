#include "render_manager_2d.h"

#include "../game/camera.h"
#include "../game/texture.h"

RenderManager2D::RenderManager2D()
{
	Setup(NULL, NULL);
}

RenderManager2D::RenderManager2D(const char *vs, const char *fs)
{
	Setup(vs, fs);
}

void RenderManager2D::Setup(const char *vs, const char *fs)
{
	shader_program = CreateShaderProgramFromPair(
			vs == NULL ? "shaders/2d.vs" : vs,
			fs == NULL ? "shaders/2d.fs" : fs
	);

	if(shader_program == NULL)
		return;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbos.model);
	glGenBuffers(1, &vbos.data);

	ModelBufferSetup();
	DataBufferSetup();
	Camera::Setup(shader_program);
}

RenderManager2D::~RenderManager2D()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &vbos.model);
	glDeleteBuffers(1, &vbos.data);
	glDeleteVertexArrays(1, &vao);
}

void RenderManager2D::ModelBufferSetup()
{
	float verts[] = {
		0.f, 1.f, 0.0f, 0.0f, 0.0f, // Top left
		1.f, 1.f, 0.0f, 1.0f, 0.0f, // Top right
		1.f, 0.f, 0.0f, 1.0f, 1.0f, // Bottom right
		1.f, 0.f, 0.0f, 1.0f, 1.0f, // Bottom right
		0.f, 0.f, 0.0f, 0.0f, 1.0f, // Bottom left
		0.f, 1.f, 0.0f, 0.0f, 0.0f  // Top left
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbos.model);
	model_attribs.verts = shader_program->GetAttribLocation("in_verts");
	model_attribs.uvs   = shader_program->GetAttribLocation("in_uvs");

	glVertexAttribPointer(model_attribs.verts, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(GL_FLOAT), 0);
	glVertexAttribPointer(model_attribs.uvs, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));

	glVertexAttribDivisor(model_attribs.verts, 0);
	glVertexAttribDivisor(model_attribs.uvs, 0);
	glEnableVertexAttribArray(model_attribs.verts);
	glEnableVertexAttribArray(model_attribs.uvs);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
}

void RenderManager2D::DataBufferSetup()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbos.data);
	data_attribs.model[0]     = shader_program->GetAttribLocation("in_modelx");
	data_attribs.model[1]     = shader_program->GetAttribLocation("in_modely");
	data_attribs.model[2]     = shader_program->GetAttribLocation("in_modelz");
	data_attribs.model[3]     = shader_program->GetAttribLocation("in_modelw");
	data_attribs.rect         = shader_program->GetAttribLocation("in_rect");
	data_attribs.colour       = shader_program->GetAttribLocation("in_colour");

	for(int i = 0; i < 4; ++i)
		glEnableVertexAttribArray(data_attribs.model[i]);
	glEnableVertexAttribArray(data_attribs.rect);
	glEnableVertexAttribArray(data_attribs.colour);

	for(int i = 0; i < 4; ++i)
		glVertexAttribPointer(data_attribs.model[i], 4, GL_FLOAT, GL_FALSE,
				sizeof(Data), (GLvoid*)(i * 4 * sizeof(GLfloat)));
	glVertexAttribPointer(data_attribs.colour, 4, GL_FLOAT, GL_FALSE,
			sizeof(Data), (GLvoid*)(16 * sizeof(GLfloat)));
	glVertexAttribPointer(data_attribs.rect, 4, GL_FLOAT, GL_FALSE,
			sizeof(Data), (GLvoid*)(20 * sizeof(GLfloat)));

	for(int i = 0; i < 4; ++i)
		glVertexAttribDivisor(data_attribs.model[i], 1);
	glVertexAttribDivisor(data_attribs.colour, 1);
	glVertexAttribDivisor(data_attribs.rect, 1);
}

void RenderManager2D::Add(Texture *tex, const glm::mat4 &model,
		const glm::vec4 &colour, const glm::vec4 &rect, int layer)
{
	data[layer][tex].push_back(Data());
	Data *d = &data[layer][tex].back();
	d->m1 = model[0];
	d->m2 = model[1];
	d->m3 = model[2];
	d->m4 = model[3];
	d->colour = colour;
	d->rect = rect;
}

void RenderManager2D::Flush(const GameTime &gametime)
{
	if(shader_program == NULL)
		return;

	glUseProgram(shader_program->GetID());

#ifdef INSOLENCE_WEBGL
	Camera::PostToShader_ES(shader_program);
#endif

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbos.model);
	glBindBuffer(GL_ARRAY_BUFFER, vbos.data);

	for(data_2d_t::reverse_iterator i = data.rbegin(); i != data.rend(); ++i)
	{
		int layer = i->first;
		Texture *tex = NULL;

		for(map_data_t::iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			if(j->second.size() == 0)
				continue;

			if(j->first != NULL)
				j->first->Bind();
			else
				glBindBuffer(GL_TEXTURE_2D, 0);

			glBufferData(GL_ARRAY_BUFFER, sizeof(Data) * j->second.size(),
					&j->second[0], GL_DYNAMIC_DRAW);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, j->second.size());
		}
	}

	data.clear();
}
