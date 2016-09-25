#ifndef COMPONENT_FONT_H
#define COMPONENT_FONT_H

#include "../insolence_dll.h"

#include "../game/log.h"
#include "../render/graphics.h"
#include "material.h"

#include <algorithm>
#include <ft2build.h>
#include FT_FREETYPE_H

enum FontType {
	NORMAL,
	PIXEL
};

namespace TextAlignH {
	enum AlignH {
		LEFT,
		RIGHT,
		CENTRE,
	};
}

namespace TextAlignV {
	enum AlignV {
		TOP,
		BOTTOM,
		CENTRE
	};
}

struct INSOLENCE_API Font {
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

	int height;
	int max_glyph_width;
	bool is_monospace;
	int kerning;

public:
	Material *mat;

	const int GetMaxGlyphWidth() const { return max_glyph_width; }
	const bool IsMonospace() const { return is_monospace; }
	const double GetAtlasWidth() const { return w; }
	const double GetAtlasHeight() const { return h; }
	const double GetLineHeight() const { return height; }
	const FontInfo& GetGlyph(char i) const { return c[i]; }

	static int GetFTFaceFromFile(const char *path, FT_Library &ft,
			FT_Face &face)
#if INSOLENCE_OPENGL_DESKTOP || INSOLENCE_WEBGL
	{
		return FT_New_Face(ft, path, 0, &face);
	}
#elif INSOLENCE_ANDROID
	{
		int ret = -1;
		FILE *file = fopen(path, "r");
		size_t file_size;
		char *buf;

		if(file == NULL)
			return ret;

		fseek(file, 0, SEEK_END);
		file_size = ftell(file);
		rewind(file);

		buf = new char[file_size + 1];
		buf[file_size] = '\0';
		fread(buf, 1, file_size, file);
		fclose(file);

		ret = FT_New_Memory_Face(ft, (FT_Byte*)buf, file_size, 0, &face);
		delete[] buf;

		return ret;
	}
#endif

	static Font* Load(const char *path, int size,
			FontType type=FontType::NORMAL)
	{
		Font *out;
		FT_Face face;
		FT_Library ft = NULL;
		FT_GlyphSlot g;
		int glyph_padding;
#if INSOLENCE_OPENGL_DESKTOP || INSOLENCE_ANDROID
		GLint gl_format = GL_RED;
#elif INSOLENCE_WEBGL
		GLint gl_format = GL_LUMINANCE;
#endif

		/* Init Freetype. */
		if(ft == NULL && FT_Init_FreeType(&ft)){
			log(Log::FATAL, "FreeType failed to initialise");
			return NULL;
		}

		if(Font::GetFTFaceFromFile(path, ft, face) != 0)
		{
			log(Log::FATAL, "Font (%d) - Couldn't generate face from %d",
				   __FUNCTION__, path);
			return NULL;
		}

		out = new Font();
		out->mat = new Material();
		out->mat->diffuse = Texture::LoadColour(glm::vec4(0.f));

		FT_Set_Pixel_Sizes(face, 0, size);
		g = face->glyph;
		out->height = face->size->metrics.height / 64.0;

		/*
		 * Texture bleeding is a problem at larger sizes and this seems like
		 * a safe bet.
		 */
		glyph_padding = std::max(size / 20, 1);

		out->w = 0;
		out->h = 0;
		out->max_glyph_width = 0;
		out->is_monospace = FT_IS_FIXED_WIDTH(face);

		/*
		 * Find out the width for the whole texture, and create an empty
		 * one of that width and height;
		 */
		for(int i = 32; i < 128; ++i)
		{
			if(FT_Load_Char(face, i, FT_LOAD_RENDER))
				continue;

			out->max_glyph_width = out->max_glyph_width < g->bitmap.width ?
				g->bitmap.width : out->max_glyph_width;

			out->w += g->bitmap.width + glyph_padding;
			out->h = std::max(out->h, (double)g->bitmap.rows);
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, out->mat->diffuse->GetID());
		glTexImage2D(GL_TEXTURE_2D, 0, gl_format, out->w,
				out->h, 0, gl_format, GL_UNSIGNED_BYTE, 0);

		/*
		 * Place each glyph into the correct place in the newly generated
		 * texture.
		 */
		int x = 0;

		for(int i = 32; i < 128; ++i)
		{
			if(FT_Load_Char(face, i, FT_LOAD_RENDER))
				continue;


			x += glyph_padding;

			out->c[i].ax = g->advance.x >> 6;
			out->c[i].ay = g->advance.y >> 6;
			out->c[i].x = x;
			out->c[i].w = g->bitmap.width;
			out->c[i].h = g->bitmap.rows;
			out->c[i].l = g->bitmap_left;
			out->c[i].t = g->bitmap_top;

			glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, out->c[i].w,
					out->c[i].h, gl_format, GL_UNSIGNED_BYTE,
					g->bitmap.buffer);

			x += out->c[i].w;
		}

		if(type == FontType::NORMAL)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
					GL_LINEAR);
		}
		else if(type == FontType::PIXEL)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
					GL_NEAREST);
		}

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
				GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
				GL_CLAMP_TO_EDGE);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		return out;
	}
};

#endif
