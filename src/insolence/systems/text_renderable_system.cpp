#include "text_renderable_system.h"

TextRenderableSystem::TextRenderableSystem(RenderManager2D* r)
:	renderer(r)
{}

void TextRenderableSystem::Manage(const GameTime& gametime)
{
	for(it = components.begin(); it!= components.end(); ++it)
	{
		TextRenderable *t = &it->second;
		Movement *m = it->first->Get<Movement>();

		if(m == NULL || t->font == 0)
			continue;

		renderer->AddText(t->font, t->text.c_str(), m->GetPositionXY(),
				t->colour, t->align, t->scale);
	}
}
