#include "font.h"

int Font::GetFTFaceFromFile(const char *path, FT_Library &ft,
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

Font* Font::Load(const char *path, int size,
		FontType type)
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
		log(Log::FATAL, "Font (%s) - Couldn't generate face from %s",
			   __FUNCTION__, path);
		return NULL;
	}

	out = new Font();
	out->mat = new Material();

	FT_Set_Pixel_Sizes(face, 0, size);
	g = face->glyph;
	out->height = face->size->metrics.height / 64.0;
	out->pixel_size = size;

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
	for(int i = 0; i < strlen(default_character_set); ++i)
	{
		if(FT_Load_Char(face, default_character_set[i], FT_LOAD_RENDER))
			continue;

		out->max_glyph_width = out->max_glyph_width < g->bitmap.width ?
			g->bitmap.width : out->max_glyph_width;

		out->w += g->bitmap.width + glyph_padding;
		out->h = std::max(out->h, (double)g->bitmap.rows);
	}

	out->mat->diffuse =
		Texture::CreateBlank(out->w, out->h, gl_format, gl_format);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, out->mat->diffuse->GetID());

	/*
	 * Place each glyph into the correct place in the newly generated
	 * texture.
	 */
	int x = 0;

	for(int i = 0; i < strlen(default_character_set); ++i)
	{
		if(FT_Load_Char(face, default_character_set[i], FT_LOAD_RENDER))
			continue;

		FontInfo *c = &out->chars_pixelsize[default_character_set[i]];
		FontInfo *cs = &out->chars_scaled[default_character_set[i]];

		x += glyph_padding;

		c->ax = g->advance.x >> 6;
		c->ay = g->advance.y >> 6;
		c->x = x;
		c->w = g->bitmap.width;
		c->h = g->bitmap.rows;
		c->l = g->bitmap_left;
		c->t = g->bitmap_top;

		cs->ax = c->ax / size;
		cs->ay = c->ay / size;
		cs->x  = c->x / size;
		cs->w  = c->w / size;
		cs->h  = c->h / size;
		cs->l  = c->l / size;
		cs->t  = c->t / size;

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, c->w,
				c->h, gl_format, GL_UNSIGNED_BYTE,
				g->bitmap.buffer);

		x += c->w;
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
