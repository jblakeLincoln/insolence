#ifndef CONSOLE_H
#define CONSOLE_H

#include <stddef.h>
#include <string>
#include <list>
#include "command_parser.h"

struct Camera;
struct Entity;
struct EntityManager;
struct Font;
struct Framebuffer;
struct Console {
private:
	/* Using floats is giving a linker error. Seems to be a clang bug. */
	static constexpr double TRANSITION_TIME = 150;
	static constexpr double FONT_SIZE = 24;
	static constexpr double PIXEL_SIZE = FONT_SIZE * 1.5;
	static const std::string PREFIX_STRING;

	/* Const until modification stuff is written. */
	const glm::vec4 caret_colour      = glm::vec4(0.2f, 0.2f, 0.6f, 1.f);
	const glm::vec4 text_colour       = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
	const glm::vec4 background_colour = glm::vec4(0.f, 0.f, 0.f, 0.7f);

	enum State {
		HIDDEN,
		VISIBLE,
	} state = HIDDEN;

	bool switching = false;
	float left_padding = 6.f;
	float bottom_padding = 6.f;

	Font *font;
	Camera *camera;
	EntityManager *mgr;
	Framebuffer *fb_console;

	std::list<Entity*> lines;
	Entity *e_current_line = nullptr;
	std::string current_line = PREFIX_STRING;

	int cumulative_lines = 0;

	Entity *caret;
	int caret_pos;
	float caret_width;

	TimeSpan transition_timer;

	void PushLine();
	void UpdateText();
	void UpdateCaret();

public:
	Console();
	~Console();

	CommandParser<> *parser;
	CommandParser<uint64_t> *entity_parser;

	void ToggleState();
	void Print(const char*, ...);
	void Draw(const GameTime &gametime);
};

#endif
