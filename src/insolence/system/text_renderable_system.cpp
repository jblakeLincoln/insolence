#include "text_renderable_system.h"

#include "../component/transform.h"
#include "../render/render_manager_2d.h"

TextRenderableSystem::TextRenderableSystem(RenderManager2D* r)
:	renderer(r)
{}

void TextRenderableSystem::Manage(const GameTime& gametime)
{
	typedef TextRenderable::Modifiers Modifiers;

	for(it = components.begin(); it!= components.end(); ++it)
	{
		if(it->second.hidden == true)
			continue;

		TextRenderable *t = &it->second;
		Transform *m = it->first->Get<Transform>();

		if(m == NULL || t->font == NULL || t->GetText().length() == 0)
			continue;

		Font *f = t->font;

		glm::mat4 model = m->GetModelMatrix();
		int len = strlen(t->GetText().c_str());
		float font_size = f->GetPixelSize();
		float longest = 0;

		std::vector<float> lengths = t->GetLineLengths();
		static bool print = true;
		for(size_t i = 0; i < lengths.size(); ++i)
			longest = std::max(longest, lengths[i]);

		float modifier_x = 0.f;
		float modifier_y = 0.f;
		float line_height = f->GetLineHeight() * t->modifiers.line_spacing;

		if(t->modifiers.halign == Modifiers::H_RIGHT)
			modifier_x = -longest;
		else if(t->modifiers.halign == Modifiers::H_CENTRE)
			modifier_x = -(longest / 2.f);

		if(t->modifiers.valign == Modifiers::V_CENTRE)
		{
			float lh = line_height / f->GetPixelSize();
			modifier_y = (lengths.size() * lh * t->modifiers.scale.y / 2.f) +
				(lh * t->modifiers.scale.y / 4.f);
		}
		else if(t->modifiers.valign == Modifiers::V_BOTTOM)
		{
			modifier_y = (line_height * (lengths.size() - 1)) /
				font_size * t->modifiers.scale.y;
		}

		if(m->GetScaleX() != 0.f)
			modifier_x += t->modifiers.offset.x * (1.f / m->GetScaleX());
		if(m->GetScaleY() != 0.f)
			modifier_y += t->modifiers.offset.y * (1.f / m->GetScaleY());

		model = glm::translate(model, glm::vec3(modifier_x, modifier_y, 0.f));
		glm::mat4 pen = model;
		const char *str = t->GetRenderText().c_str();

		for(int i = 0, line_count = 0; i < len; ++i)
		{
			if(i == 0 || str[i] == '\n')
			{
				float x = 0;
				if(t->modifiers.halign == Modifiers::H_CENTRE)
					x = (longest / 2.f) - (lengths[line_count] / 2.f);
				else if(t->modifiers.halign == Modifiers::H_RIGHT)
					x = longest - lengths[line_count];

				x *= (f->GetPixelSize() / t->modifiers.scale.x);
				++line_count;

				int lc = line_count;

				if(t->modifiers.valign == Modifiers::V_BOTTOM)
					lc = line_count - 1;

				pen = glm::translate(model, glm::vec3(
							x,
							-(line_height * lc) / font_size *
								t->modifiers.scale.y,
							0.f));

				if(str[i] == '\n')
					continue;
			}

			const Font::FontInfo *glyph = &f->GetGlyph(str[i]);
			glm::mat4 glyph_model = pen;
			glm::vec4 rect = glm::vec4(
					glyph->x / f->GetAtlasWidth(),
					glyph->y / f->GetAtlasHeight(),
					glyph->w / f->GetAtlasWidth(),
					glyph->h / f->GetAtlasHeight());

			glyph_model = glm::translate(glyph_model, glm::vec3(
						glyph->l * t->modifiers.scale.x,
						(glyph->t - glyph->h) * t->modifiers.scale.y,
						0.f) / font_size);

			glyph_model = glm::scale(glyph_model, glm::vec3(
						glyph->w * t->modifiers.scale.x,
						glyph->h * t->modifiers.scale.y,
						1.f) / font_size);

			renderer->Add(f->mat->diffuse, glyph_model, t->colour, rect);

			if(i == len - 1)
				break;

			pen = glm::translate(pen,
					glm::vec3(glyph->ax, 0.f, 0.f) / font_size * t->modifiers.scale.y);
		}
	}
}
