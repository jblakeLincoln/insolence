#include "render_manager_simple_particles.h"

#include "../game/camera.h"

RenderManagerSimpleParticles::RenderManagerSimpleParticles(ShaderProgram *prog)
{
	if(prog == nullptr) {
		log(Log::FATAL, "RenderManagerSimpleParticles - Failed to create "
				"shader program");
		return;
	}

	float verts[] = {
		0.f, 1.f, // Top left
		1.f, 1.f, // Top right
		1.f, 0.f, // Bottom right
		1.f, 0.f, // Bottom right
		0.f, 0.f, // Bottom left
		0.f, 1.f // Top left
	};

	shader_program = prog;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo_verts);
	glGenBuffers(1, &vbo_data);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
	attrib_verts = glGetAttribLocation(shader_program->GetID(), "in_verts");

	glVertexAttribPointer(attrib_verts, 2, GL_FLOAT, GL_FALSE,
			2 * sizeof(GL_FLOAT), 0);
	glVertexAttribDivisor(attrib_verts, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);
	glEnableVertexAttribArray(attrib_verts);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_data);
	attrib_pos = glGetAttribLocation(shader_program->GetID(), "in_pos");
	attrib_colour = glGetAttribLocation(shader_program->GetID(), "in_colour");
	attrib_scale= glGetAttribLocation(shader_program->GetID(), "in_scale");

	glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
			7 * sizeof(GL_FLOAT), 0);
	glVertexAttribPointer(attrib_colour, 4, GL_FLOAT, GL_FALSE,
			7 * sizeof(GL_FLOAT), (GLvoid*)(2 * sizeof(GLfloat)));
	glVertexAttribPointer(attrib_scale, 1, GL_FLOAT, GL_FALSE,
			7 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GLfloat)));

	glVertexAttribDivisor(attrib_pos, 1);
	glVertexAttribDivisor(attrib_colour, 1);
	glVertexAttribDivisor(attrib_scale, 1);
	glEnableVertexAttribArray(attrib_pos);
	glEnableVertexAttribArray(attrib_colour);
	glEnableVertexAttribArray(attrib_scale);

	Camera::Setup(shader_program);
}

RenderManagerSimpleParticles::~RenderManagerSimpleParticles()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo_verts);
	glDeleteBuffers(1, &vbo_data);
	glDeleteVertexArrays(1, &vao);
}

void RenderManagerSimpleParticles::Add(float *data_in, int count)
{
	data[data_in] = count;
}

void RenderManagerSimpleParticles::Flush(const GameTime &gametime)
{
	glUseProgram(shader_program->GetID());
#ifdef INSOLENCE_WEBGL
	Camera::PostToShader_ES(shader_program);
#endif

	for(std::unordered_map<float*, uint32_t>::iterator it = data.begin();
			it != data.end(); ++it)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_data);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * it->second * 7,
				&it->first[0],
				GL_DYNAMIC_DRAW);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, it->second);
	}

	data.clear();
}
