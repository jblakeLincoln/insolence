#ifndef COMPONENT_TEXT_RENDERABLE_H
#define COMPONENT_TEXT_RENDERABLE_H

#include "../insolence_dll.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <string>

#include <float.h>

#include "component.h"
#include "../game/font.h"

struct TextRenderableSystem;

/**
 * TextRenderable stores two versions of its string to render - the provided
 * representation of the text, and the visual representation with line breaks
 * after applying any specified line wrapping.
 *
 * line_lengths and line_counts represent visual lengths and character count
 * for the visual representation.
 */
struct INSOLENCE_API TextRenderable : Component {
private:
	std::vector<float> line_lengths;
	std::vector<int> line_counts;

	std::string text;
	std::string render_text;

	enum {
		NONE         = 1 << 0,
		TEXT_CHANGED = 1 << 1
	};

	uint32_t flags;

	void UpdateRenderText();

public:
	const std::string& GetText() const {
		return text;
	}

	void SetText(const char *t) {
		flags |= TEXT_CHANGED;
		text = t;
	}

	const std::string& GetRenderText() {
		UpdateRenderText();
		return render_text;
	}
	const std::vector<float>& GetLineLengths() {
		UpdateRenderText();
		return line_lengths;
	}

	const std::vector<int>& GetLineCounts() {
		UpdateRenderText();
		return line_counts;
	}

	struct Modifiers {
		friend TextRenderable;
	protected:
		float max_width = FLT_MAX;
	public:
		Modifiers() {}

		enum AlignH {
			H_LEFT,
			H_RIGHT,
			H_CENTRE,
		} halign = H_LEFT;

		enum AlignV {
			V_TOP,
			V_BOTTOM,
			V_CENTRE
		} valign = V_BOTTOM;

		glm::vec2 scale = glm::vec2(1.f);
		glm::vec2 offset = glm::vec2(0.f);
		float line_spacing = 1.f;

	} modifiers;

	float GetMaxWidth() const {
		return modifiers.max_width;
	}

	void SetMaxWidth(float f) {
		flags |= TEXT_CHANGED;
		modifiers.max_width = f;
	}

	Font *font;
	glm::vec4 colour;
	bool hidden;

	TextRenderable(Font *font,
			const std::string &str   = "",
			const glm::vec4& colour  = glm::vec4(1.f),
			const Modifiers &mod = Modifiers())
		: font(font)
		, colour(colour)
		, modifiers(mod)
		, hidden(false)
	{
		flags |= TEXT_CHANGED;
		SetText(str.c_str());
	}
};

#endif
