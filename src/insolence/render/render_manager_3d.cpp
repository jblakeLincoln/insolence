#include "render_manager_3d.h"

#include "../game/camera.h"
#include "../game/material.h"
#include "../game/mesh.h"

RenderManager3D::RenderManager3D()
{
	shader_program = CreateShaderProgramFromPair(
			"shaders/3d.vs",
			"shaders/3d.fs");

	if(shader_program == NULL)
		return;

	dir_light_uniforms.direction =
		shader_program->GetUniformLocation("dir_light.direction");
	dir_light_uniforms.diffuse =
		shader_program->GetUniformLocation("dir_light.diffuse");
	dir_light_uniforms.ambient =
		shader_program->GetUniformLocation("dir_light.ambient");
	dir_light_uniforms.specular =
		shader_program->GetUniformLocation("dir_light.specular");

	uniforms.view_pos      = shader_program->GetUniformLocation("view_pos");
	uniforms.tex_diffuse   = shader_program->GetUniformLocation("tex_diffuse");
	uniforms.tex_normal    = shader_program->GetUniformLocation("tex_normal");
	uniforms.is_normal_map = shader_program->GetUniformLocation("normal_map");

	mesh_attribs.verts    = shader_program->GetAttribLocation("in_vert");
	mesh_attribs.norms    = shader_program->GetAttribLocation("in_norm");
	mesh_attribs.uvs      = shader_program->GetAttribLocation("in_uv");

	data_attribs.model[0] = shader_program->GetAttribLocation("in_modelx");
	data_attribs.model[1] = shader_program->GetAttribLocation("in_modely");
	data_attribs.model[2] = shader_program->GetAttribLocation("in_modelz");
	data_attribs.model[3] = shader_program->GetAttribLocation("in_modelw");
	data_attribs.colour   = shader_program->GetAttribLocation("in_colour");

	glUniform1i(uniforms.tex_diffuse, 0);
	glUniform1i(uniforms.tex_normal, 1);

	point_light_uniforms[0].pos =
		shader_program->GetUniformLocation("point_lights[0].pos");
	point_light_uniforms[0].constant =
		shader_program->GetUniformLocation("point_lights[0].constant");
	point_light_uniforms[0].linear =
		shader_program->GetUniformLocation("point_lights[0].linear");
	point_light_uniforms[0].quadratic =
		shader_program->GetUniformLocation("point_lights[0].quadratic");
	point_light_uniforms[0].diffuse =
		shader_program->GetUniformLocation("point_lights[0].diffuse");
	point_light_uniforms[0].ambient =
		shader_program->GetUniformLocation("point_lights[0].ambient");
	point_light_uniforms[0].specular =
		shader_program->GetUniformLocation("point_lights[0].specular");

	Camera::Setup(shader_program);
}

RenderManager3D::~RenderManager3D()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for(mesh_table_t::iterator i = meshes.begin(); i != meshes.end(); ++i)
	{
		glDeleteBuffers(1, &i->second.mesh);
		glDeleteBuffers(1, &i->second.model);
		glDeleteVertexArrays(1, &i->second.vao);
	}
}

void RenderManager3D::Flush(const GameTime &gametime)
{
	if(shader_program == NULL)
		return;

	glm::vec3 view_pos;

#ifdef INSOLENCE_WEBGL
	Camera::PostToShader_ES(shader_program);
#endif

	glUseProgram(shader_program->GetID());

	if(Camera::GetActiveCamera() != NULL)
		view_pos = Camera::GetActiveCamera()->pos.GetPos();

	glUniform3fv(uniforms.view_pos,            1, &view_pos[0]);
	glUniform3fv(dir_light_uniforms.direction, 1, &dir_light.direction[0]);
	glUniform3fv(dir_light_uniforms.diffuse,   1, &dir_light.diffuse[0]);
	glUniform3fv(dir_light_uniforms.ambient,   1, &dir_light.ambient[0]);
	glUniform3fv(dir_light_uniforms.specular,  1, &dir_light.specular[0]);

	int updated_light_count = 0;
	for(std::list<PointLight*>::iterator i = point_lights.begin();
			i != point_lights.end(); ++i)
	{
		UpdatePointLight(**i, updated_light_count++);
	}

	for(mesh_table_t::iterator i = meshes.begin(); i != meshes.end(); ++i)
	{
		for(material_table_t::iterator j = data[i->first].begin();
				j != data[i->first].end(); ++j)
		{
			Mesh *mesh = i->first;
			MeshBuffers *buffers = &i->second;
			Material *mat = j->first;
			const std::vector<Data> &d = j->second;

			if(mat == NULL)
				continue;

			glActiveTexture(GL_TEXTURE0);
			if(mat->diffuse != NULL)
				mat->diffuse->Bind();
			else
				glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE1);
			if(mat->normal != NULL)
			{
				mat->normal->Bind();
				glUniform1i(uniforms.is_normal_map, true);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(uniforms.is_normal_map, false);
			}

			glBindVertexArray(buffers->vao);
			glBindBuffer(GL_ARRAY_BUFFER, buffers->model);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Data) * d.size(), &d[0],
					GL_DYNAMIC_DRAW);
			glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->num_verts, d.size());
		}
	}

	data.clear();
}

RenderManager3D::PointLight* RenderManager3D::RequestPointLight()
{
	if(point_lights.size() >= RenderManager3D::MAX_POINT_LIGHTS)
	{
		log(Log::WARN, "RenderManager3D (%s): Attempting to allocate more "
				"PointLights than permitted", __FUNCTION__);
		return NULL;
	}

	PointLight *l = new PointLight();
	point_lights.push_back(l);

	return l;
}

void RenderManager3D::UpdatePointLight(const PointLight &l, int index)
{
	/*
	 * This works under the assumption that uniform locations of an array are
	 * contiguous. There's no reason to assume that's not the case, but
	 * assumptions sometimes hurt, so here's a note of it.
	 */
	/*
	 * TODO: glUniform functions look like they can update the whole array at
	 * once. Investigate.
	 */

	int count = sizeof(PointLightUniforms) / sizeof(GLint) * index;

	glUniform3fv(point_light_uniforms[0].pos + count, 1,
			&(l).pos[0]);
	glUniform1fv(point_light_uniforms[0].constant + count, 1,
			&(l).constant);
	glUniform1fv(point_light_uniforms[0].linear + count, 1,
			&(l).linear);
	glUniform1fv(point_light_uniforms[0].quadratic + count, 1,
			&(l).quadratic);
	glUniform3fv(point_light_uniforms[0].diffuse + count, 1,
			&(l).diffuse[0]);
	glUniform3fv(point_light_uniforms[0].ambient + count, 1,
			&(l).ambient[0]);
	glUniform3fv(point_light_uniforms[0].specular + count, 1,
			&(l).specular[0]);
}

void RenderManager3D::FreePointLight(PointLight *l)
{
	glUseProgram(shader_program->GetID());
	l->constant = 0;
	UpdatePointLight(*l, point_lights.size() - 1);
	point_lights.remove(l);

	delete l;
}

void RenderManager3D::CreateMeshBuffer(Mesh *m)
{
	MeshBuffers *buffers = &meshes[m];

	glGenVertexArrays(1, &buffers->vao);
	glBindVertexArray(buffers->vao);

	glGenBuffers(1, &buffers->mesh);
	glGenBuffers(1, &buffers->model);

	/*
	 * Model attributes
	 */
	glBindBuffer(GL_ARRAY_BUFFER, buffers->model);

	glEnableVertexAttribArray(data_attribs.model[0]);
	glEnableVertexAttribArray(data_attribs.model[1]);
	glEnableVertexAttribArray(data_attribs.model[2]);
	glEnableVertexAttribArray(data_attribs.model[3]);
	glEnableVertexAttribArray(data_attribs.colour);

	for(int i = 0; i < 4; ++i)
		glVertexAttribPointer(data_attribs.model[i], 4, GL_FLOAT, GL_FALSE,
				sizeof(Data), (GLvoid*)(i * 4 * sizeof(GLfloat)));
	glVertexAttribPointer(data_attribs.colour, 4, GL_FLOAT, GL_FALSE,
			sizeof(Data), (GLvoid*)(16 * sizeof(GLfloat)));

	for(int i = 0; i < 4; ++i)
		glVertexAttribDivisor(data_attribs.model[i], 1);
	glVertexAttribDivisor(data_attribs.colour, 1);

	/*
	 * Mesh attributes
	 */
	glBindBuffer(GL_ARRAY_BUFFER, buffers->mesh);
	glEnableVertexAttribArray(mesh_attribs.verts);
	glEnableVertexAttribArray(mesh_attribs.norms);
	glEnableVertexAttribArray(mesh_attribs.uvs);

	glVertexAttribPointer(mesh_attribs.verts, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(GL_FLOAT), 0);
	glVertexAttribPointer(mesh_attribs.norms, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(mesh_attribs.uvs, 2, GL_FLOAT, GL_FALSE,
			8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GLfloat)));

	glVertexAttribDivisor(mesh_attribs.verts, 0);
	glVertexAttribDivisor(mesh_attribs.norms, 0);
	glVertexAttribDivisor(mesh_attribs.uvs, 0);

	/* Post mesh buffer. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (m->num_verts * 8),
			m->data, GL_STATIC_DRAW);
}

void RenderManager3D::Add(Mesh *mesh, Material *material,
		const glm::mat4 &model, const glm::vec4 &colour)
{
	if(mesh != NULL && meshes.find(mesh) == meshes.end())
		CreateMeshBuffer(mesh);
	else if(meshes.size() > 0 && mesh == NULL)
		mesh = meshes.begin()->first;

	Data d;
	d.model = model;
	d.colour = colour;
	data[mesh][material].push_back(d);
}
