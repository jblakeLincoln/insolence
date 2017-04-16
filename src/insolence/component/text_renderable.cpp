#include "text_renderable.h"

#include <glm/glm.hpp>
#include <float.h>
#include <string.h>

void TextRenderable::UpdateRenderText()
{
	if((flags & TEXT_CHANGED) != TEXT_CHANGED)
		return;
	flags &= ~TEXT_CHANGED;

	char *str = strdup(text.c_str());
	float max_length = FLT_MAX;

	if(modifiers.max_width != FLT_MAX)
		max_length = modifiers.max_width * (font->GetPixelSize() / modifiers.scale.x);

	line_lengths = {};
	line_counts = {};

	char *str_ptr = str;
	float line_length = 0;
	int line_num_chars = 0;

	/*
	 * Iterate over the string word by word. Insert a line break whenever the
	 * next word exceeds the maximum line length. Store the glyph lengths and
	 * character counts of each line.
	 *
	 * TODO: Support line breaks properly, and generally rewrite to actually
	 * not be horrible.
	 */
	while(true)
	{
		const char *newline_ptr = strchr(str_ptr, '\n');
		const char *nul_ptr = strchr(str_ptr, '\0');
		const char *space_ptr = strchr(str_ptr, ' ');
		const char *next_break = nul_ptr;
		intptr_t next_break_ptr = (intptr_t)nul_ptr;

		if(newline_ptr != nullptr)
			next_break_ptr = glm::min((intptr_t)next_break, (intptr_t)newline_ptr);
		if(space_ptr != nullptr)
			next_break_ptr = glm::min((intptr_t)next_break, (intptr_t)space_ptr);
		next_break = (const char*)next_break_ptr;

		float word_length = 0;
		int word_num_chars = next_break - str_ptr;

		for(size_t i = 0; i < next_break - str_ptr + 1; ++i)
			word_length += font->GetGlyph(str_ptr[i]).ax;

		if(line_length + word_length >= max_length)
		{
			if(line_length > 0)
				*(str_ptr - 1) = '\n';
			else
			{
				line_length = word_length;
				line_num_chars = word_num_chars;
			}

			line_lengths.push_back(line_length);
			line_counts.push_back(line_num_chars);
			line_length = 0;
			line_num_chars = 0;
		}

		if(*(str_ptr + (int)word_num_chars) == '\n' ||
				*(str_ptr + (int)word_num_chars) == '\0')
		{
			float actual_line_length = line_length + word_length;
			int actual_line_num_chars = line_num_chars + word_num_chars;

			if(line_length + word_length < max_length)
			{
				word_length = 0;
				word_num_chars = 0;
			}

			line_lengths.push_back(actual_line_length);
			line_counts.push_back(actual_line_num_chars);

			line_length = 0;
			line_num_chars = 0;
		}
		else
		{
			line_length += word_length;
			line_num_chars += word_num_chars;
		}

		if(space_ptr == nullptr && newline_ptr == nullptr)
			break;
		++line_num_chars;
		str_ptr += word_num_chars + 1;
	}

	//for(size_t i = 0; i < line_lengths.size(); ++i)
	//	line_lengths[i] = line_lengths[i] / font->GetPixelSize() * modifiers.scale.x;

	render_text = str;
	free(str);
}

