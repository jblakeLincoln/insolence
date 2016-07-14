#include "default_renderers.h"

DefaultRenderer* DefaultRenderer::renderer_instance;

DefaultRenderer::DefaultRenderer()
{
	Renderer2D = new RenderManager2D();
	Renderer3D = new RenderManager3D();
	RendererText = new RenderManager2D("shaders/2d.vs", "shaders/text.fs");
}

DefaultRenderer::~DefaultRenderer()
{
	delete Renderer2D;
	delete Renderer3D;
	delete RendererText;
}

void DefaultRenderer::Flush()
{
	Renderer2D->Flush();
	RendererText->Flush();
	Renderer3D->Flush();
}