#include "render_manager_3d.h"

#include "../game/camera.h"
#include "../game/material.h"
#include "../game/mesh.h"

RenderManager3D::RenderManager3D()
{
	shader_program = CreateShaderProgramFromPair("shaders/3d.vs",
			"shaders/3d.fs");

	if(shader_program == NULL)
	{
		log(Log::FATAL, "RenderManager3D - Failed to create shader program");
		return;
	}

	uni_projection = glGetUniformLocation(shader_program->GetID(), "mat_proj");
	uni_view = glGetUniformLocation(shader_program->GetID(), "mat_view");
	uni_direction = glGetUniformLocation(shader_program->GetID(),
			"dir_light.direction");
	uni_diffuse = glGetUniformLocation(shader_program->GetID(),
			"dir_light.diffuse");
	uni_ambient = glGetUniformLocation(shader_program->GetID(),
			"dir_light.ambient");
	uni_specular = glGetUniformLocation(shader_program->GetID(),
			"dir_light.specular");
	uni_view_pos = glGetUniformLocation(shader_program->GetID(),
			"view_pos");

	dir_light.direction = glm::vec3(2.f, -6.f, -6.f);
	dir_light.diffuse = glm::vec3(2.f);
	dir_light.specular = glm::vec3(0.0f);
	dir_light.ambient = glm::vec3(0.0f);

	glUniform1i(glGetUniformLocation(shader_program->GetID(), "tex_diffuse"),
			0);
	glUniform1i(glGetUniformLocation(shader_program->GetID(), "tex_normal"),
			1);

	is_normal_map = glGetUniformLocation(shader_program->GetID(),
			"normal_map");

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Camera::Setup(shader_program);
}

RenderManager3D::~RenderManager3D()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for(mesh_def::iterator i = mesh_buffers.begin(); i != mesh_buffers.end();
			++i)
	{
		glDeleteBuffers(1, &i->second.vbo_mesh);
		glDeleteBuffers(1, &i->second.vbo_model);
		glDeleteVertexArrays(1, &i->second.vao);
	}
}

void RenderManager3D::Flush()
{
	/*
	 * This looks pretty awful ... but it works to a reasonable standard
	 * (admittedly on the low end of reasonable), so it stays for now.
	 */

#ifdef INSOLENCE_OPENGL_ES
	Camera::PostToShader_ES(shader_program);
#endif

	if(Camera::GetActiveCamera() != NULL)
		SetViewPosition(Camera::GetActiveCamera()->pos.GetPos());
	glUseProgram(shader_program->GetID());

	glUniform3fv(uni_view_pos, 1, &view_pos[0]);
	glUniform3fv(uni_direction, 1, &dir_light.direction[0]);
	glUniform3fv(uni_diffuse, 1, &dir_light.diffuse[0]);
	glUniform3fv(uni_ambient, 1, &dir_light.ambient[0]);
	glUniform3fv(uni_specular, 1, &dir_light.specular[0]);

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

			if(j->first == NULL)
				continue;

			glActiveTexture(GL_TEXTURE0);

			if(j->first->diffuse != NULL)
				j->first->diffuse->Bind();
			else
				glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE1);
			if(j->first->normal != NULL)
			{
				j->first->normal->Bind();
				glUniform1i(is_normal_map, true);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(is_normal_map, false);
			}


			glBindVertexArray(i->second.vao);

			glBindBuffer(GL_ARRAY_BUFFER, i->second.vbo_model);

			glBufferData(GL_ARRAY_BUFFER, sizeof(float) *
					j->second.size(),
					&j->second[0], GL_DYNAMIC_DRAW);

			glDrawArraysInstanced(GL_TRIANGLES, 0,
					i->first->num_verts,
					j->second.size() / 20);

			glBindVertexArray(0);
		}
	}

	mesh_data.clear();
}

void RenderManager3D::Add(Mesh *m, Material *t, const glm::vec4& colour,
		const glm::mat4& mat)
{
	/* If we don't have a VBO for this mesh, create it. */
	if(mesh_buffers.find(m) == mesh_buffers.end())
	{
		mesh_data_buffers data;
		glGenVertexArrays(1, &data.vao);
		glBindVertexArray(data.vao);

		glGenBuffers(1, &data.vbo_mesh);
		glGenBuffers(1, &data.vbo_model);

		glBindBuffer(GL_ARRAY_BUFFER, data.vbo_model);

		attrib_models[0] = glGetAttribLocation(
				shader_program->GetID(), "in_modelx");
		attrib_models[1] = glGetAttribLocation(
				shader_program->GetID(), "in_modely");
		attrib_models[2] = glGetAttribLocation(
				shader_program->GetID(), "in_modelz");
		attrib_models[3] = glGetAttribLocation(
				shader_program->GetID(), "in_modelw");

		attrib_colour = glGetAttribLocation(
				shader_program->GetID(), "in_colour");

		/* Vertex attribs. */
		int vbo_attrib_len = 20 * sizeof(GL_FLOAT);

		for(int i = 0; i < 4; ++i)
			glEnableVertexAttribArray(attrib_models[i]);

		glEnableVertexAttribArray(attrib_colour);

		for(int i = 0; i < 4; ++i)
		{
			glVertexAttribPointer(attrib_models[i], 4, GL_FLOAT, GL_FALSE,
				vbo_attrib_len, (GLvoid*)(i * 4 * sizeof(GLfloat)));
		}

		glVertexAttribPointer(attrib_colour, 4, GL_FLOAT, GL_FALSE,
			vbo_attrib_len, (GLvoid*)(16 * sizeof(GLfloat)));

		for(int i = 0; i < 4; ++i)
			glVertexAttribDivisor(attrib_models[i], 1);

		glVertexAttribDivisor(attrib_colour, 1);

		glBindBuffer(GL_ARRAY_BUFFER, data.vbo_mesh);

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

		mesh_buffers[m] = data;

		glBindVertexArray(0);
	}

	/* Update our to-draw model matrix list for this mesh and texture. */
	int index = mesh_data[m][t].size();
	mesh_data[m][t].resize(mesh_data[m][t].size() + 20);

	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			mesh_data[m][t][index + (i * 4 + j)] = mat[i][j];

	for(int i = 0; i < 4; ++i)
		mesh_data[m][t][index + 16 + i] = colour[i];

}

void RenderManager3D::SetViewPosition(const glm::vec3& vp)
{
	glUseProgram(shader_program->GetID());
	view_pos = vp;
}
