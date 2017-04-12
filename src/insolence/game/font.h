#ifndef COMPONENT_FONT_H
#define COMPONENT_FONT_H

#include "../insolence_dll.h"

#include "../game/log.h"
#include "../render/graphics.h"
#include "material.h"

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H

#include <unordered_map>

enum FontType {
	NORMAL,
	PIXEL
};

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
	static constexpr char const *default_character_set =
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"1234567890!\"£$%^&*€()"
		"-=_+[]{};'#:@~,./<>?\\|¬"
		" \0";

	std::unordered_map<char, FontInfo> chars_pixelsize;
	std::unordered_map<char, FontInfo> chars_scaled;

	double w;
	double h;

	double height;
	int max_glyph_width;
	bool is_monospace;
	int kerning;
	int pixel_size;

	static int GetFTFaceFromFile(const char *path, FT_Library&, FT_Face &);

public:
	Material *mat;

	const int GetMaxGlyphWidth() const { return max_glyph_width; }
	const bool IsMonospace() const { return is_monospace; }
	const double GetAtlasWidth() const { return w; }
	const double GetAtlasHeight() const { return h; }
	const double GetLineHeight() const { return height; }
	const double GetPixelSize() const { return pixel_size; }
	const FontInfo& GetGlyph(char i) { return chars_pixelsize[i]; }
	const FontInfo& GetScaledGlyph(char i) { return chars_scaled[i]; }

	static Font* Load(const char *path, int size, FontType = FontType::NORMAL);
};

#endif
