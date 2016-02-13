#include "render_manager_3d.h"

RenderManager3D::RenderManager3D()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_data);
	glBindVertexArray(vao);

	shader_program = CreateShaderProgramFromPair("shaders/3d.vs",
			"shaders/3d.fs");

	if(shader_program == NULL)
	{
		// Error out.
	}

	uni_projection = glGetUniformLocation(shader_program->GetID(), "mat_proj");
	uni_view = glGetUniformLocation(shader_program->GetID(), "mat_view");

	glBindBuffer(GL_ARRAY_BUFFER, vbo_data);

	attrib_models[0] = glGetAttribLocation(
			shader_program->GetID(), "in_modelx");
	attrib_models[1] = glGetAttribLocation(
			shader_program->GetID(), "in_modely");
	attrib_models[2] = glGetAttribLocation(
			shader_program->GetID(), "in_modelz");
	attrib_models[3] = glGetAttribLocation(
			shader_program->GetID(), "in_modelw");

	/* Vertex attribs. */
	int vbo_attrib_len = 16 * sizeof(GL_FLOAT);

	for(int i = 0; i < 4; ++i)
		glEnableVertexAttribArray(attrib_models[i]);

	for(int i = 0; i < 4; ++i)
	{
		glVertexAttribPointer(attrib_models[i], 4, GL_FLOAT, GL_FALSE,
			vbo_attrib_len, (GLvoid*)(i * 4 * sizeof(GLfloat)));
	}

	for(int i = 0; i < 4; ++i)
		glVertexAttribDivisor(attrib_models[i], 1);

	glBindVertexArray(0);
}

RenderManager3D::~RenderManager3D()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for(mesh_def::iterator i = mesh_buffers.begin(); i != mesh_buffers.end();
			++i)
		glDeleteBuffers(1, &i->second);
	glDeleteBuffers(1, &vbo_data);

	glDeleteVertexArrays(1, &vao);
}

void RenderManager3D::Manage()
{
	/*
	 * This looks pretty awful ... but it works to a reasonable standard
	 * (admittedly on the low end of reasonable), so it stays for now.
	 */

	glBindVertexArray(vao);

	for(mesh_def::iterator i = mesh_buffers.begin(); i != mesh_buffers.end();
			++i)
	{
		for(tex_def::iterator j = mesh_data[i->first].begin(); j !=
				mesh_data[i->first].end(); ++j)
		{
			/*
			 * i->first		=	Mesh.
			 * i->second	=	Mesh VBO.
			 * j->first		=	Texture.
			 * j->second	=	Vector of model matrix floats.
			 *
			 * It's almost like these iterator variables should have names
			 * that represent what they are or something.
			 */

			j->first->Bind();

			glBindBuffer(GL_ARRAY_BUFFER, vbo_data);

			glBufferData(GL_ARRAY_BUFFER, sizeof(float) *
					j->second.size(),
					&j->second[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, i->second);

			glDrawArraysInstanced(GL_TRIANGLES, 0,
					i->first->num_verts,
					j->second.size() / 16);
		}
	}

	mesh_data.clear();
}

void RenderManager3D::Add(Mesh *m, Texture *t, const glm::mat4& mat)
{
	glBindVertexArray(vao);

	/* If we don't have a VBO for this mesh, create it. */
	if(mesh_buffers.find(m) == mesh_buffers.end())
	{
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		/*
		 * Set up where mesh data will be fed.
		 * TODO Issue #22: Add colour data and whatever else.
		 */

		GLint attrib_verts =
			glGetAttribLocation(shader_program->GetID(), "in_vert");
		GLint attrib_norms =
			glGetAttribLocation(shader_program->GetID(), "in_norm");
		GLint attrib_uvs =
			glGetAttribLocation(shader_program->GetID(), "in_uv");

		glVertexAttribPointer(attrib_verts, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(GL_FLOAT), 0);
		glVertexAttribPointer(attrib_norms, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
		glVertexAttribPointer(attrib_uvs, 2, GL_FLOAT, GL_FALSE,
			8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GLfloat)));

		glVertexAttribDivisor(attrib_norms, 0);
		glVertexAttribDivisor(attrib_verts, 0);
		glVertexAttribDivisor(attrib_uvs, 0);

		glEnableVertexAttribArray(attrib_verts);
		glEnableVertexAttribArray(attrib_uvs);
		glEnableVertexAttribArray(attrib_norms);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (m->num_verts * 8),
			m->data, GL_STATIC_DRAW);

		mesh_buffers[m] = vbo;
	}

	/* Update our to-draw model matrix list for this mesh and texture. */
	int index = mesh_data[m][t].size();
	mesh_data[m][t].resize(mesh_data[m][t].size() + 16);

	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			mesh_data[m][t][index + (i * 4 + j)] = mat[i][j];

}
