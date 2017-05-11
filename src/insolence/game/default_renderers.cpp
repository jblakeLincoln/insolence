#include "default_renderers.h"

#include "game.h"

DefaultRenderer* DefaultRenderer::renderer_instance;

DefaultRenderer::DefaultRenderer()
{
	Renderer2D = new RenderManager2D(
			Game::Get()->resources.shader_programs.Get(
				EngineResources::ShaderProgram::DEFAULT_2D));
	Renderer3D = new RenderManager3D(
			Game::Get()->resources.shader_programs.Get(
				EngineResources::ShaderProgram::DEFAULT_3D));
	RendererText = new RenderManager2D(
			Game::Get()->resources.shader_programs.Get(
				EngineResources::ShaderProgram::DEFAULT_TEXT));
}

DefaultRenderer::~DefaultRenderer()
{
	delete Renderer2D;
	delete Renderer3D;
	delete RendererText;
}

void DefaultRenderer::Flush(const GameTime &gametime)
{
	Renderer2D->Flush(gametime);
	RendererText->Flush(gametime);
	Renderer3D->Flush(gametime);
}
