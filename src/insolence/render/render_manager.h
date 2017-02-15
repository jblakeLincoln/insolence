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

	RenderManager() {}
	virtual ~RenderManager();

	virtual void Flush(const GameTime &gametime)=0;

	static ShaderProgram* CreateShaderProgramFromPair(const char *vert_path,
			const char *frag_path);
};

#endif
