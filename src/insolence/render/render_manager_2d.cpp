#include "render_manager_2d.h"

#include "../game/camera.h"
#include "../game/texture.h"

RenderManager2D::RenderManager2D(const char *vs, const char *fs)
{
	glm::vec3 view_pos = glm::vec3(2.f, 0.f, 2.f);
	int vbo_attrib_len;

	/* Data for our quad - centre aligned - 3 verts, 2 UVs. */
	/*
	float verts[] = {
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, // Top left
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Top right
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // Bottom right
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // Bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom left
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f // Top left
	};
	*/
	float verts[] = {
		0.f, 1.f, 0.0f, 0.0f, 0.0f, // Top left
		1.f, 1.f, 0.0f, 1.0f, 0.0f, // Top right
		1.f, 0.f, 0.0f, 1.0f, 1.0f, // Bottom right
		1.f, 0.f, 0.0f, 1.0f, 1.0f, // Bottom right
		0.f, 0.f, 0.0f, 0.0f, 1.0f, // Bottom left
		0.f, 1.f, 0.0f, 0.0f, 0.0f // Top left
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo_verts);
	glGenBuffers(1, &vbo_data);

	if(vs == NULL && fs == NULL)
		shader_program = CreateShaderProgramFromPair("shaders/2d.vs",
				"shaders/2d.fs");
	else
		shader_program = CreateShaderProgramFromPair(vs, fs);

	if(shader_program == NULL)
	{
		log(Log::FATAL, "RenderManager2D - Failed to create shader program");
		return;
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

#ifdef INSOLENCE_WEBGL
	Camera::PostToShader_ES(shader_program);
#endif

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_data);

	for(def_layer_data::reverse_iterator i = data.rbegin();
			i != data.rend(); ++i)
	{
		int layer = i->first;
		Texture *tex = NULL;

		for(def_tex_data::iterator j = data[layer].begin();
				j != data[layer].end(); ++j)
		{
			tex = j->first;

			if(count[layer][tex] == 0)
				continue;

			if(tex != NULL)
				tex->Bind();

			glBufferData(GL_ARRAY_BUFFER, sizeof(float) *
					(count[layer][tex] * 24),
					&data[layer][tex][0],
					GL_DYNAMIC_DRAW);

			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count[layer][tex]);
		}
	}

	data.clear();
	count.clear();
}

void RenderManager2D::Add(Texture *t, const glm::mat4& model,
		const glm::vec4& colour, int layer=0)
{
	if(count.count(layer) == 0)
		count[layer][t] = 0;

	/*
	 * TODO Issue #15: Fix all this copying - should everything always be
	 * stored in the renderer?
	 */

	data[layer][t].resize((count[layer][t]+2048)*24);

	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			data[layer][t][count[layer][t] * 24 + (i * 4 + j)] = model[i][j];

	/* Way bad. */
	/*
	if(data[t][count[t] * 24 + 14] == 0)
		data[t][count[t] * 24 + 14] = count[t] * 0.1;
	*/


	for(int i = 0; i < 4; ++i)
		data[layer][t][count[layer][t] * 24 + 16 + i] = (colour[i]);

	count[layer][t]++;
}

void RenderManager2D::Add(Texture *t, const glm::mat4&model,
		const glm::vec4& colour, const glm::vec4& rect, int layer=0)
{
	Add(t, model, colour, layer);

	--count[layer][t];

	for(int i = 0; i < 4; ++i)
		data[layer][t][count[layer][t]* 24 + 20 + i] = rect[i];

	++count[layer][t];
}


void RenderManager2D::AddText(Font *f, const char *str, const glm::vec2& pos,
		const glm::vec4& col, TextAlignH::AlignH halign,
		TextAlignV::AlignV valign, glm::vec2 scale)
{
	float offset_h = 0;
	float offset_v = 0;
	float length = 0;
	int line_count = 0;
	std::vector<float> lengths;
	std::vector<int> charcounts;

	int total_chars = 0;

	/* Calculate the length of the string in pixels. */
	for(int i = 0, total_chars = 0; i < strlen(str); ++i)
	{
		const Font::FontInfo c = f->GetScaledGlyph(str[i]);

		if(str[i] != '\n')
			length += c.l + c.ax;
		else
		{
			length -= c.ax;
			length -= c.l;
			length += c.w;
		}

		if(str[i] == '\n' || i == strlen(str) - 1)
		{
			int line_chars = i - total_chars;

			if(str[i] != '\n')
				++line_chars;

			charcounts.push_back(line_chars);
			lengths.push_back(length * scale.x);
			total_chars = i + 1;
			length = 0;
		}
	}

	if(Camera::GetCoordinateSystem() == Camera::Coords::Y_DOWN)
		scale.y = -scale.y;

	if(valign == TextAlignV::CENTRE)
		offset_v = f->GetLineHeight() * lengths.size() / 2.f;
	else if(valign == TextAlignV::TOP)
		offset_v = f->GetLineHeight() * lengths.size();
	offset_v /= f->GetPixelSize();
	offset_v *= scale.y;

	for(int i = 0, line_count = 0; i < strlen(str); ++i)
	{
		const Font::FontInfo glyph = f->GetGlyph(str[i]);
		const Font::FontInfo glyph_scaled = f->GetScaledGlyph(str[i]);
		glm::mat4 m;
		glm::vec3 p;
		char c = str[i];
		float aspect = 1;

		if(str[i] == '\n' || i == 0)
		{
			double new_offset_v;
			length = lengths[line_count];

			if(f->IsMonospace() == true)
			{
				length = charcounts[line_count] * f->GetMaxGlyphWidth() /
					f->GetPixelSize() * scale.x;
			}
			++line_count;

			if(halign == TextAlignH::CENTRE)
				offset_h = -length / 2.0;
			else if(halign == TextAlignH::RIGHT)
				offset_h = -length;
			else if(halign == TextAlignH::LEFT)
				offset_h = 0;

			if((valign == TextAlignV::BOTTOM && line_count > 0) == false)
				new_offset_v = -f->GetLineHeight();

			new_offset_v /= f->GetPixelSize();
			new_offset_v *= scale.y;

			offset_v += new_offset_v;

			if(str[i] == '\n')
				continue;
		}

		if(f->IsMonospace() == true)
			offset_h += f->GetMaxGlyphWidth() / f->GetPixelSize() / 2.0 * scale.x;
		else
			offset_h += glyph_scaled.l * scale.x;


		if(glyph.w != 0)
			aspect = glyph.h / glyph.w;

		p.x = offset_h + pos.x;
		p.y = -glyph.h + glyph.t * 2.f  - (1 - glyph.h) - (glyph.t * 2.0) + pos.y + offset_v;
		p.y = -glyph.h + glyph.t + pos.y + offset_v;

		m = glm::translate(m, p);
		m = glm::scale(m, glm::vec3(glyph_scaled.w * scale.x,
					glyph_scaled.h * scale.y,
					1.f));

		glm::vec4 rect = glm::vec4(glyph.x / f->GetAtlasWidth(),
				0,
				glyph.w / f->GetAtlasWidth(),
				glyph.h / f->GetAtlasHeight());

		Add(f->mat->diffuse, m, col, rect);

		if(f->IsMonospace() == true)
			offset_h += f->GetMaxGlyphWidth() / f->GetPixelSize() / 2.0 * scale.x;
		else
			offset_h += glyph_scaled.ax * scale.x;
	}
}
