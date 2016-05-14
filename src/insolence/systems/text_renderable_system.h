#ifndef SYSTEM_TEXT_RENDERABLE_H
#define SYSTEM_TEXT_RENDERABLE_H

#include "../insolence_dll.h"

#include "../game/entity.h"
#include "../component/text_renderable.h"
#include "../render/render_manager_2d.h"

struct INSOLENCE_API TextRenderableSystem : System<TextRenderable> {
private:
	RenderManager2D* renderer;

public:
	TextRenderableSystem(RenderManager2D*);
	void Manage(const GameTime&);
};

#endif
