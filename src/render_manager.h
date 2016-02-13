#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "shader_program.h"
#include <glm/matrix.hpp>

/* TODO Issue #13: Logging. */
struct RenderManager
{
	ShaderProgram *shader_program;
	glm::mat4 mat_projection;
	glm::mat4 mat_view;
	GLint uni_projection;
	GLint uni_view;

	/**
	 * The renderer drawing at the end of the frame.
	 */
	virtual void Manage()=0;

	RenderManager() {}
	~RenderManager();

	/**
	 * Creates a shader program from a vertex/frag pair.
	 *
	 * \param vert_path	Filepath of vertex shader.
	 * \param frag_path	Filepath of fragment shader.
	 */
	static ShaderProgram* CreateShaderProgramFromPair(const char *vert_path,
			const char *frag_path)
	{
		Shader **shaders = new Shader*[2];
		ShaderProgram *program = new ShaderProgram();

		shaders[0] = Shader::CreateFromFile(vert_path, GL_VERTEX_SHADER);
		shaders[1] = Shader::CreateFromFile(frag_path, GL_FRAGMENT_SHADER);

		for(int i = 0; i < 2; ++i)
		{
			// Log status/failure;
			glAttachShader(program->GetID(), shaders[i]->GetID());
		}

		glBindFragDataLocation(program->GetID(), 0, "out_colour");
		glLinkProgram(program->GetID());
		glUseProgram(program->GetID());

		for(int i = 0; i < 2; ++i)
			delete shaders[i];

		delete[] shaders;

		// Log success

		return program;
	}
};

#endif
