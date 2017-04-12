#ifndef CONSOLE_H
#define CONSOLE_H

#include <stddef.h>
#include <string>
#include <vector>
#include "../game/entity_manager.h"

struct Camera;
struct Font;
struct Framebuffer;
struct Console {
	/* Using floats is giving a linker error. Seems to be a clang bug. */
	static constexpr double TRANSITION_TIME = 150;
	static constexpr double FONT_SIZE = 24;
	static constexpr double PIXEL_SIZE = FONT_SIZE * 1.5;
	static const std::string PREFIX_STRING;

	enum State {
		HIDDEN,
		VISIBLE,
	} state = HIDDEN;

	bool switching = false;
	float left_padding = 6.f;
	float bottom_padding = 6.f;

	Camera *camera;
	EntityManager *mgr;
	Framebuffer *fb_console;
	Font *font;

	std::vector<Entity*> lines;
	std::string current_line = PREFIX_STRING;
	std::vector<std::string> lines_list;

	Entity *caret;
	int caret_pos;
	float caret_width;

	TimeSpan transition_timer;

	void UpdateText();
	void UpdateCaret();

public:
	Console();
	~Console();

	void ToggleState();
	void Draw(const GameTime &gametime);
};

#endif
