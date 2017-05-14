#ifndef SYSTEM_TEXT_RENDERABLE_H
#define SYSTEM_TEXT_RENDERABLE_H

#include "../insolence_dll.h"

#include "../component/text_renderable.h"

struct RenderManager2D;
template<>
struct INSOLENCE_API System<TextRenderable> : SystemBase<TextRenderable> {
private:
	RenderManager2D* renderer;

public:
	System<TextRenderable>() {
		renderer = Game::Get()->renderers.text;
	}

	void Manage(const GameTime&);
};

#endif
