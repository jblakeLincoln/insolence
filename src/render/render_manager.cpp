#include "render_manager.h"

RenderManager::~RenderManager()
{
	if(shader_program != NULL)
		delete shader_program;
}
