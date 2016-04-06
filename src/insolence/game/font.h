#ifndef COMPONENT_FONT_H
#define COMPONENT_FONT_H

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include "material.h"

enum FontAlign {
	LEFT,
	RIGHT,
	CENTRE,
};

struct Font {
	/* Struct of individual glyph information. */
	struct FontInfo {
		/* Advance X/Y. */
		double ax;
		double ay;

		/* Rectangle pos and dimensions */
		double x;
		double y;
		double w;
		double h;

		/* Left/top. */
		double l;
		double t;
	};

private:
	FontInfo c[128];
	double w;
	double h;

public:
	Material *mat;

	const double GetAtlasWidth() const { return w; }
	const double GetAtlasHeight() const { return h; }
	const FontInfo& GetGlyph(char i) const { return c[i]; }

	static Font* Load(const char *path, int size)
	{
		Font *out = new Font();

		FT_Library ft;
		FT_Face face;

		/* Init Freetype. */
		if(FT_Init_FreeType(&ft)){
			// Log FreeType failed.
			exit(1);
		}

		out->mat = new Material();
		out->mat->diffuse = Texture::LoadColour(glm::vec4(0.f));

		if(FT_New_Face(ft, path, 0, &face))
		{
			// Log can't load font.
			delete out;
			return NULL;
		}

		FT_Set_Pixel_Sizes(face, 0, size);
		FT_GlyphSlot g = face->glyph;

		out->w = 0;
		out->h = 0;

		/*
		 * Find out the width for the whole texture, and create an empty
		 * one of that width and height;
		 */
		for(int i = 32; i < 128; ++i)
		{
			if(FT_Load_Char(face, i, FT_LOAD_RENDER))
				continue;

			out->w += g->bitmap.width + 1;
			out->h = std::max(out->h, (double)g->bitmap.rows);
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, out->mat->diffuse->GetID());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, out->w,
				out->h, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 0);

		/*
		 * Place each glyph into the correct place in the newly generated
		 * texture.
		 */
		int x = 0;

		for(int i = 32; i < 128; ++i)
		{
			if(FT_Load_Char(face, i, FT_LOAD_RENDER))
				continue;

			x += 1;

			out->c[i].ax = g->advance.x >> 6;
			out->c[i].ay = g->advance.y >> 6;
			out->c[i].x = x;
			out->c[i].w = g->bitmap.width;
			out->c[i].h = g->bitmap.rows;
			out->c[i].l = g->bitmap_left;
			out->c[i].t = g->bitmap_top;

			/* Go over our font data and change it to render white. */
			GLubyte *buf = new GLubyte[2 * g->bitmap.width * g->bitmap.rows];
			for(int j = 0; j < g->bitmap.rows; ++j)
			{
				for(int i = 0; i < g->bitmap.width; ++i)
				{
					buf[2 * (i + j * g->bitmap.width)] = 255;
					buf[2 * (i + j * g->bitmap.width) + 1] =
						(i >= g->bitmap.width || j >= g->bitmap.rows) ? 0 :
						g->bitmap.buffer[i + g->bitmap.width * j];
				}
			}

			glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, out->c[i].w,
					out->c[i].h, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,
					buf);

			delete[] buf;

			x += out->c[i].w;
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_LINEAR_MIPMAP_LINEAR);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		return out;
	}
};

#endif
