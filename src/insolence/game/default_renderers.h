#ifndef DEFAULT_RENDERERS_H
#define DEFAULT_RENDERERS_H

#include "../render/render_manager_2d.h"
#include "../render/render_manager_3d.h"

struct DefaultRenderer
{
private:
	static DefaultRenderer *renderer_instance;

	DefaultRenderer();
	~DefaultRenderer();

public:
	RenderManager2D *Renderer2D;
	RenderManager3D *Renderer3D;
	RenderManager2D *RendererText;

	void Flush();

	static DefaultRenderer *Get()
	{
		if(DefaultRenderer::renderer_instance == NULL)
			DefaultRenderer::renderer_instance = new DefaultRenderer();
		return renderer_instance;
	}
};

#endif
