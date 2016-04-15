#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "../game/camera.h"
#include "../game/log.h"
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
	virtual void Flush()=0;

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
		bool success = true;

		shaders[0] = Shader::CreateFromFile(vert_path, GL_VERTEX_SHADER);
		shaders[1] = Shader::CreateFromFile(frag_path, GL_FRAGMENT_SHADER);

		for(int i = 0; i < 2; ++i)
		{
			GLint status = shaders[i]->GetShaderiv(GL_COMPILE_STATUS);
			glAttachShader(program->GetID(), shaders[i]->GetID());

			if(status != GL_TRUE)
			{
				log(Log::ERR, "RenderManager (%s) - shader %d compile failed",
						__FUNCTION__, i);
			}

			success = status & success;
		}

		if(success == false)
		{
			for(int i = 0; i < 2; ++i)
			{
				char buf[1024];
				int length = 0;
				glGetShaderInfoLog(shaders[i]->GetID(), 1024, &length, buf);

				if(length == 0)
					continue;

				buf[length-1] = '\0';

				log(Log::ERR, "RenderManager (%s) - Shader %d log: %s",
						__FUNCTION__, i, buf);
			}
		}

		glBindFragDataLocation(program->GetID(), 0, "out_colour");
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
