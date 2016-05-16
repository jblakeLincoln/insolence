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

		if(m == NULL || t->font == NULL || t->text == NULL)
			continue;

		renderer->AddText(t->font, t->text, m->GetPositionXY(),
				t->colour, t->align, t->scale);
	}
}
