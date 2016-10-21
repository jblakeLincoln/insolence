#include "text_renderable_system.h"

#include "../component/transform.h"
#include "../render/render_manager_2d.h"

TextRenderableSystem::TextRenderableSystem(RenderManager2D* r)
:	renderer(r)
{}

void TextRenderableSystem::Manage(const GameTime& gametime)
{
	for(it = components.begin(); it!= components.end(); ++it)
	{
		if(it->second.hidden == true)
			continue;

		TextRenderable *t = &it->second;
		Transform *m = it->first->Get<Transform>();

		if(m == NULL || t->font == NULL || t->text == NULL)
			continue;

		renderer->AddText(t->font, t->text, m->GetPosXY(),
				t->colour, t->halign, t->valign, t->scale * m->GetScaleXY());
	}
}
