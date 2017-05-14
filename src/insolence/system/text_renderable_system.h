#ifndef SYSTEM_TEXT_RENDERABLE_H
#define SYSTEM_TEXT_RENDERABLE_H

#include "../insolence_dll.h"

#include "../component/text_renderable.h"

struct RenderManager2D;
struct INSOLENCE_API TextRenderableSystem : System<TextRenderable> {
private:
	RenderManager2D* renderer;

public:
	TextRenderableSystem();
	void Manage(const GameTime&);
};

#endif
