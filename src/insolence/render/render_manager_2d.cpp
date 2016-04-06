#include "render_manager_2d.h"

RenderManager2D::RenderManager2D()
{
	glm::vec3 view_pos = glm::vec3(2.f, 0.f, 2.f);
	int vbo_attrib_len;

	/* Data for our quad - centre aligned - 3 verts, 2 UVs. */
	float verts[] = {
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, // Top left
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Top right
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // Bottom right
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // Bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom left
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f // Top left
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo_verts);
	glGenBuffers(1, &vbo_data);

	shader_program = CreateShaderProgramFromPair("shaders/2d.vs",
			"shaders/2d.fs");

	if(shader_program == NULL)
	{
		// Error out.
	}

	uni_projection = glGetUniformLocation(shader_program->GetID(), "mat_proj");
	uni_view = glGetUniformLocation(shader_program->GetID(), "mat_view");

	/* Set up and bind all of our data that will be reused for each sprite. */
	glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
	attrib_verts = glGetAttribLocation(shader_program->GetID(), "in_verts");
	attrib_uv = glGetAttribLocation(shader_program->GetID(), "in_uvs");
	glVertexAttribPointer(attrib_verts, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(GL_FLOAT), 0);
	glVertexAttribPointer(attrib_uv, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));

	glVertexAttribDivisor(attrib_verts, 0);
	glVertexAttribDivisor(attrib_uv, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);

	/* Prepare our buffers for the per-entity data. */
	glBindBuffer(GL_ARRAY_BUFFER, vbo_data);
	attrib_models[0] = glGetAttribLocation(shader_program->GetID(),
			"in_modelx");
	attrib_models[1] = glGetAttribLocation(shader_program->GetID(),
			"in_modely");
	attrib_models[2] = glGetAttribLocation(shader_program->GetID(),
			"in_modelz");
	attrib_models[3] = glGetAttribLocation(shader_program->GetID(),
			"in_modelw");
	attrib_colour = glGetAttribLocation(shader_program->GetID(), "in_colour");
	attrib_model_uv = glGetAttribLocation(shader_program->GetID(), "in_rect");

	glEnableVertexAttribArray(attrib_verts);
	glEnableVertexAttribArray(attrib_uv);
	glEnableVertexAttribArray(attrib_model_uv);
	glEnableVertexAttribArray(attrib_colour);

	for(int i = 0; i < 4; ++i)
		glEnableVertexAttribArray(attrib_models[i]);
	/*
	 * m1-4 + colour + uvs
	 * (4*4) + 4 + 4 = 24
	 */
	vbo_attrib_len = 24 * sizeof(GL_FLOAT);
	for(int i = 0; i < 4; ++i)
		glVertexAttribPointer(attrib_models[i], 4, GL_FLOAT, GL_FALSE,
			vbo_attrib_len, (GLvoid*)(i * 4 * sizeof(GLfloat)));

	glVertexAttribPointer(attrib_colour, 4, GL_FLOAT, GL_FALSE,
			vbo_attrib_len, (GLvoid*)(16 * sizeof(GLfloat)));

	glVertexAttribPointer(attrib_model_uv, 4, GL_FLOAT, GL_FALSE,
			vbo_attrib_len, (GLvoid*)(20 * sizeof(GLfloat)));

	for(int i = 0; i < 4; ++i)
		glVertexAttribDivisor(attrib_models[i], 1);

	glVertexAttribDivisor(attrib_colour, 1);
	glVertexAttribDivisor(attrib_model_uv, 1);

	Camera::Setup(shader_program);
}

RenderManager2D::~RenderManager2D()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo_verts);
	glDeleteBuffers(1, &vbo_data);
	glDeleteVertexArrays(1, &vao);
}

void RenderManager2D::Flush()
{
	glUseProgram(shader_program->GetID());

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_data);

	for(data_def::iterator i = data.begin(); i != data.end(); ++i)
	{
		if(count[i->first] == 0)
			continue;

		if(i->first != NULL)
			i->first->Bind();

		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(count[i->first]*24),
				&data[i->first][0],
			GL_DYNAMIC_DRAW);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count[i->first]);
	}

	data.clear();
	count.clear();
}

void RenderManager2D::Add(Texture *t, const glm::mat4& model,
		const glm::vec4& colour)
{
	if(count.count(t) == 0)
		count[t] = 0;

	/*
	 * TODO Issue #15: Fix all this copying - should everything always be
	 * stored in the renderer?
	 */

	data[t].resize((count[t]+2048)*24);

	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			data[t][count[t] * 24 + (i * 4 + j)] = model[i][j];

	/* Way bad. */
	/*
	if(data[t][count[t] * 24 + 14] == 0)
		data[t][count[t] * 24 + 14] = count[t] * 0.1;
	*/


	for(int i = 0; i < 4; ++i)
		data[t][count[t] * 24 + 16 + i] = (colour[i]);

	count[t]++;
}

void RenderManager2D::Add(Texture *t, const glm::mat4&model,
		const glm::vec4& colour, const glm::vec4& rect)
{
	Add(t, model, colour);

	--count[t];

	for(int i = 0; i < 4; ++i)
		data[t][count[t]* 24 + 20 + i] = rect[i];

	++count[t];
}

void RenderManager2D::AddText(Font *f, const char *str, const glm::vec2& pos,
		const glm::vec4& col, FontAlign origin, glm::vec2 scale)
{
	float offset = 0;
	float length = 0;

	if(scale == glm::vec2())
		scale = glm::vec2(1.f);

	/* Calculate the length of the string in pixels. */
	for(int i = 0; i < strlen(str); ++i)
	{
		const Font::FontInfo c = f->GetGlyph(str[i]);

		length +=
			c.l + c.ax;

		if(i == strlen(str)-1)
		{
			length -= c.ax;
			length -= c.l;
			length += c.w;
		}
	}

	if(origin == FontAlign::CENTRE)
		offset -= length / 2.0;
	else if (origin == FontAlign::RIGHT)
		offset += length;

	for(int i = 0; i < strlen(str); ++i)
	{
		glm::mat4 m;
		const Font::FontInfo glyph = f->GetGlyph(str[i]);
		char c = str[i];

		offset += glyph.w * scale.x/2;
		offset += glyph.l * scale.x;

		m = glm::translate(m, glm::vec3(offset + pos.x,
					((f->GetAtlasHeight() - glyph.h) / f->GetAtlasHeight() +
					 glyph.t - (glyph.h/2)) * scale.y + pos.y,
					0.f));

		m = glm::scale(m, glm::vec3(glyph.w * scale.x,
					glyph.h * scale.y, 1.f));

		glm::vec4 rect = glm::vec4(glyph.x/f->GetAtlasWidth(),
				0,
				glyph.w/f->GetAtlasWidth(),
				glyph.h / f->GetAtlasHeight());

		Add(f->mat->diffuse, m, col, rect);

		offset += glyph.ax * scale.x;
		offset -= glyph.w * scale.x / 2;
	}
}
