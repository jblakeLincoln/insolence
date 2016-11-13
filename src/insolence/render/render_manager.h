#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "../insolence_dll.h"

#include "../game/log.h"
#include "shader_program.h"
#include "../game/gametime.h"

#include <glm/matrix.hpp>

/* TODO Issue #13: Logging. */
struct INSOLENCE_API RenderManager
{
	ShaderProgram *shader_program;
	glm::mat4 mat_projection;
	glm::mat4 mat_view;
	GLint uni_projection;
	GLint uni_view;

	/**
	 * The renderer drawing at the end of the frame.
	 */
	virtual void Flush(const GameTime &gametime)=0;

	RenderManager() {}
	virtual ~RenderManager();

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

		shaders[0] = Shader::CreateFromFile(vert_path, GL_VERTEX_SHADER,
				ShaderFlags::ADD_HEADER);
		shaders[1] = Shader::CreateFromFile(frag_path, GL_FRAGMENT_SHADER,
				ShaderFlags::ADD_HEADER);

		for(int i = 0; i < 2; ++i)
		{
			if(shaders[i]->GetCompileStatus() != GL_TRUE)
			{
				shaders[i]->LogCompileInfo("RenderManager (%s) - "
						"%s ", __FUNCTION__, i == 0 ? "Vert" : "Frag");
			}
			else
				glAttachShader(program->GetID(), shaders[i]->GetID());
		}

#ifdef INSOLENCE_OPENGL_DESKTOP
		glBindFragDataLocation(program->GetID(), 0, "out_colour");
#endif
		glLinkProgram(program->GetID());
		glUseProgram(program->GetID());

		for(int i = 0; i < 2; ++i)
			delete shaders[i];

		delete[] shaders;

		log(Log::INFO, "RenderManager (%s) - Successfully created pair",
				__FUNCTION__);

		return program;
	}
};

#endif
