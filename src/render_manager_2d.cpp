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

	/* TODO Issue #16: Update once we have a camera class. */
	mat_projection = glm::perspective(1.f, 1.f, 0.1f, 10000.f);
	mat_view = glm::lookAt(view_pos, glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));

	uni_projection = glGetUniformLocation(shader_program->GetID(), "mat_proj");
	uni_view = glGetUniformLocation(shader_program->GetID(), "mat_view");

	glUniformMatrix4fv(uni_projection, 1, GL_FALSE, &mat_projection[0][0]);
	glUniformMatrix4fv(uni_view, 1, GL_FALSE, &mat_view[0][0]);

	/* Set up and bind all of our data that will be reused for each sprite. */
	glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
	attrib_verts = glGetAttribLocation(shader_program->GetID(), "in_verts");
	attrib_uvs = glGetAttribLocation(shader_program->GetID(), "in_uvs");
	glVertexAttribPointer(attrib_verts, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(GL_FLOAT), 0);
	glVertexAttribPointer(attrib_uvs, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribDivisor(attrib_verts, 0);
	glVertexAttribDivisor(attrib_uvs, 0);
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

	glEnableVertexAttribArray(attrib_verts);
	glEnableVertexAttribArray(attrib_uvs);
	glEnableVertexAttribArray(attrib_colour);

	for(int i = 0; i < 4; ++i)
		glEnableVertexAttribArray(attrib_models[i]);
	/*
	 * m1-4 + colour
	 * (4*4) + 4 = 20
	 */
	vbo_attrib_len = 20 * sizeof(GL_FLOAT);
	for(int i = 0; i < 4; ++i)
		glVertexAttribPointer(attrib_models[i], 4, GL_FLOAT, GL_FALSE,
			vbo_attrib_len, (GLvoid*)(i * 4 * sizeof(GLfloat)));

	glVertexAttribPointer(attrib_colour, 4, GL_FLOAT, GL_FALSE,
			vbo_attrib_len, (GLvoid*)(16 * sizeof(GLfloat)));

	for(int i = 0; i < 4; ++i)
		glVertexAttribDivisor(attrib_models[i], 1);
	glVertexAttribDivisor(attrib_colour, 1);

	count = 0;

	/* TODO Issue #14: Decide on allocation. Copying FNA for now. */
	for(int i = 0; i < 2048*20; ++i)
		data.push_back(0);
}

RenderManager2D::~RenderManager2D()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo_verts);
	glDeleteBuffers(1, &vbo_data);
	glDeleteVertexArrays(1, &vao);
}

void RenderManager2D::Manage()
{
	glBindVertexArray(vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(count*20), &data[0],
			GL_DYNAMIC_DRAW);

	if(count == 0)
		return;

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);

	count = 0;
}

void RenderManager2D::Add(const glm::mat4& model, const glm::vec4& colour)
{
	/*
	 * TODO Issue #15: Fix all this copying - should everything always be
	 * stored in the renderer?
	 */

	/* Can't decide whether resize is faster than looping push_back. */
	if(count*20 >= data.size())
		data.resize((count+2048)*20);

	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			data[count*20+(i*4+j)] = model[i][j];

	for(int i = 0; i < 4; ++i)
		data[count*20+16+i] = (colour[i]);

	count++;
}
