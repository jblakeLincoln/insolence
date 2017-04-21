#include "text_renderable.h"

#include <glm/glm.hpp>
#include <float.h>
#include <string.h>

void TextRenderable::UpdateRenderText()
{
	if((flags & TEXT_CHANGED) != TEXT_CHANGED)
		return;
	flags &= ~TEXT_CHANGED;

	const char *str = text.c_str();
	float max_length = FLT_MAX;

	if(modifiers.max_width != FLT_MAX)
		max_length = modifiers.max_width * (font->GetPixelSize() / modifiers.scale.x);

	line_lengths = {};
	line_counts = {};

	float line_length = 0;
	int line_num_chars = 0;

	struct {
		float glyphs_len = 0;
		int char_count = 0;
	} current_word, current_line;

	bool wrap_by_word = true;

	/*
	 * Iterate over the string either character by character or word by word,
	 * depending on whether the skip flag is set. Insert a line break whenever
	 * the next word or character exceeds the maximum line length. Store the
	 * glyph lengths and character line_counts of each line.
	 *
	 * TODO: Fix wrapping so that the first word exceeding the line limit
	 *       doesn't prevent the wrap of the second word.
	 */
	for(size_t i = 0; i < text.length() + 1; ++i) {
		char c = text[i];
		current_word.glyphs_len += font->GetGlyph(str[i]).ax;
		++current_word.char_count;

		if(wrap_by_word == true && text[i] != ' ' && text[i] != '\n' && text[i] != '\0')
			continue;

		bool exceeds_max =
			current_line.glyphs_len + current_word.glyphs_len > max_length;

		if((exceeds_max == false ||
					(exceeds_max == true && current_line.char_count == 0)))
		{
			current_line.glyphs_len += current_word.glyphs_len;
			current_line.char_count += current_word.char_count;
			current_word.glyphs_len = 0;
			current_word.char_count = 0;

			if(text[i] != '\0' && text[i] != '\n')
				continue;
		}

		line_lengths.push_back(current_line.glyphs_len);
		line_counts.push_back(current_line.char_count);

		current_line.glyphs_len = 0;
		current_line.char_count = 0;

		if(text[i] == '\0' && current_word.char_count != 0) {
			line_lengths.push_back(current_word.glyphs_len);
			line_counts.push_back(current_word.char_count);
		}
	}
}
