#include "render_manager.h"

RenderManager::~RenderManager()
{
	if(shader_program != NULL)
		delete shader_program;
}

ShaderProgram* RenderManager::CreateShaderProgramFromPair(const char *vert_path,
		const char *frag_path)
{
	Shader *shaders[2];
	ShaderProgram *program = new ShaderProgram();

	shaders[0] = Shader::CreateFromFile(vert_path, GL_VERTEX_SHADER,
			ShaderFlags::ADD_HEADER);
	shaders[1] = Shader::CreateFromFile(frag_path, GL_FRAGMENT_SHADER,
			ShaderFlags::ADD_HEADER);

	bool success = true;
	for(int i = 0; i < 2; ++i)
	{
		if(shaders[i]->GetCompileStatus() != GL_TRUE)
			success = false;
		else
			glAttachShader(program->GetID(), shaders[i]->GetID());

		shaders[i]->LogCompileInfo("RenderManager (%s) - "
				"%s ", __FUNCTION__, i == 0 ? "Vert" : "Frag");
	}

#ifdef INSOLENCE_OPENGL_DESKTOP
	glBindFragDataLocation(program->GetID(), 0, "out_colour");
#endif

	if(success == false)
	{
		delete program;
		return NULL;
	}

	glLinkProgram(program->GetID());
	glUseProgram(program->GetID());

	for(int i = 0; i < 2; ++i)
		delete shaders[i];

	log(Log::INFO, "RenderManager (%s) - Successfully created pair",
			__FUNCTION__);

	return program;

}
