#include "console.h"

#include "math.h"
#include <string.h>
#include "../game/camera.h"
#include "../game/font.h"
#include "../render/framebuffer.h"
#include "../system/sprite_renderable_system.h"

const std::string Console::PREFIX_STRING = "$ ";

Console::Console()
{
	camera = new Camera(Window::GetActive(), Camera::ORTHO);

	if(Camera::GetActive() == camera)
		Camera::SetActive(nullptr);

	mgr = new EntityManager();
	font = Font::Load("samples_assets/ProFontWindows.ttf", PIXEL_SIZE);

	parser = new CommandParser<>();
	entity_parser = new CommandParser<uint64_t>();
	bottom_padding = (font->GetLineHeight() - font->GetGlyph('W').h) / PIXEL_SIZE * FONT_SIZE;

	mgr->AddRenderSystem<SpriteRenderableSystem>(
			DefaultRenderer::Get()->Renderer2D);
	mgr->AddRenderSystem<TextRenderableSystem>(
			DefaultRenderer::Get()->RendererText);

	fb_console = Framebuffer::Create(Window::GetActive());

	caret_pos = PREFIX_STRING.length();
	caret_width = (font->GetScaledGlyph('W').ax) * FONT_SIZE;
	caret = mgr->CreateEntity();
	caret->Add<SpriteRenderable>();
	caret->Get<Transform>()->SetScaleXY(caret_width, FONT_SIZE);
	caret->Get<SpriteRenderable>()->colour = caret_colour;
	caret->Get<Transform>()->SetPosY(bottom_padding / 2.f);

	PushLine();
	UpdateCaret();
}

Console::~Console()
{
	delete fb_console;
	delete camera;
	delete mgr;
	delete font;
}

void Console::ToggleState()
{
	if(switching == true)
		return;

	switching = true;
	state = state == VISIBLE ? HIDDEN : VISIBLE;
}

void Console::UpdateCaret()
{
	if(Input::GetKey(JKEY_KEY_LCTRL))
	{
		/* Go to previous word. */
		if(Input::GetKey(JKEY_KEY_LEFT) == JKEY_PRESS)
		{
			char current_char = current_line[caret_pos];
			int index = caret_pos;
			while(current_char == ' ' && --index > 0)
				current_char = current_line[index];
			current_line[index] = '\0';
			const char *res = strrchr(current_line.c_str(), ' ');
			current_line[index] = current_char;
			if(res != nullptr)
				caret_pos = std::max((size_t)(res - current_line.c_str() + 1), PREFIX_STRING.length());
		}
		/* Go to next word. */
		else if(Input::GetKey(JKEY_KEY_RIGHT) == JKEY_PRESS)
		{
			int index = caret_pos;
			while(current_line[index] == ' ' && index < current_line.length())
				++index;
			const char *res = strchr(current_line.c_str() + index, ' ');
			if(res != nullptr)
				caret_pos = std::min((size_t)(res - current_line.c_str()), current_line.length());
			else
				caret_pos = current_line.length();
		}
		else if(Input::GetKey(JKEY_KEY_C) == JKEY_PRESS)
		{
			current_line += "^C";
			PushLine();
		}
	}

	/* Go to line start. */
	if(Input::GetKey(JKEY_KEY_HOME) == JKEY_PRESS)
		caret_pos = PREFIX_STRING.length();
	/* Go to line end. */
	if(Input::GetKey(JKEY_KEY_END) == JKEY_PRESS)
		caret_pos = current_line.length();

	glm::vec2 vis_caret_pos;
	int caret_pos_in_line = 0;

	const std::vector<int> &line_counts
		= e_current_line->Get<TextRenderable>()->GetLineCounts();

	bool movedown = false;

	for(size_t i = 0; i < line_counts.size(); ++i) {
		if(caret_pos_in_line + line_counts[i] >= caret_pos)
		{
			/*
			 * If we're at the beginning of a line, stop the caret going back up
			 * */
			if(caret_pos_in_line + line_counts[i] == caret_pos &&
					line_counts.size() != 1 &&
					caret_pos_in_line + line_counts[i] != current_line.length())
				movedown = true;
			break;
		}
		caret_pos_in_line += line_counts[i];
		++vis_caret_pos.y;
	}

	for(size_t i = caret_pos_in_line; i < caret_pos; ++i) {
		const Font::FontInfo &glyph = font->GetScaledGlyph(current_line[i]);
		vis_caret_pos.x += glyph.ax * FONT_SIZE;
	}

	if(movedown == true) {
		++vis_caret_pos.y;
		vis_caret_pos.x = 0;
	}

	caret->Get<Transform>()->SetPosXY(vis_caret_pos.x + left_padding,
			-(vis_caret_pos.y * FONT_SIZE) + (bottom_padding / 2) -
			(FONT_SIZE * cumulative_lines));
}

void Console::UpdateText()
{
	bool modification = false;
	int prev_caret_pos = caret_pos;

	if(state != VISIBLE)
		return;

	if(Input::GetKey(JKEY_KEY_ENTER) == JKEY_PRESS)
		modification = true;

	current_line.insert(caret_pos, Window::text_input);
	caret_pos += Window::text_input.length();

	if(Input::GetKey(JKEY_KEY_LEFT) == JKEY_PRESS && caret_pos > PREFIX_STRING.length())
		--caret_pos;
	if(Input::GetKey(JKEY_KEY_RIGHT) == JKEY_PRESS && caret_pos < current_line.length())
		++caret_pos;

	if(Input::GetKey(JKEY_KEY_BACKSPACE) == JKEY_PRESS && caret_pos > PREFIX_STRING.length()) {
		current_line.erase(caret_pos - 1, 1);
		--caret_pos;
	}

	if(Input::GetKey(JKEY_KEY_DEL) == JKEY_PRESS && caret_pos < current_line.length())
		current_line.erase(caret_pos, 1);

	e_current_line->Get<TextRenderable>()->SetText(current_line.c_str());

	if(modification == true)
	{
		std::string command_to_parse = current_line.c_str() + PREFIX_STRING.length();
		PushLine();

		if(parser->ParseCommand(command_to_parse.c_str()) == -1)
			Print("Command '%s' not found", parser->GetLastCommand());
	}

	UpdateCaret();
}

void Console::Print(const char *fmt, ...)
{
	va_list args;
	char *line;

	va_start(args, fmt);

	if(vasprintf(&line, fmt, args) < 0)
		return;
	va_end(args);

	Entity *e = mgr->CreateEntity();

	e->Add<TextRenderable>(font);
	e->Get<TextRenderable>()->colour = text_colour;
	e->Get<TextRenderable>()->modifiers.scale = glm::vec2(FONT_SIZE);
	e->Get<TextRenderable>()->modifiers.valign = TextRenderable::Modifiers::V_TOP;
	e->Get<TextRenderable>()->SetMaxWidth(640.f - caret->Get<Transform>()->GetScaleX() - left_padding);
	e->Get<Transform>()->SetPosX(left_padding);
	e->Get<Transform>()->SetPosY(FONT_SIZE + bottom_padding - (FONT_SIZE * cumulative_lines));// + FONT_SIZE);
	e->Get<TextRenderable>()->SetText(line);

	cumulative_lines += e->Get<TextRenderable>()->GetLineLengths().size();
	lines.push_back(e);

	if(e_current_line != nullptr)
		e_current_line->Get<Transform>()->SetPosY(FONT_SIZE + bottom_padding - (FONT_SIZE * cumulative_lines));// + FONT_SIZE);
}

void Console::PushLine()
{
	if(e_current_line != nullptr) {
		e_current_line->Get<TextRenderable>()->SetText(current_line.c_str());
		cumulative_lines += e_current_line->Get<TextRenderable>()->GetLineLengths().size();
		lines.push_back(e_current_line);
	}

	current_line = PREFIX_STRING;
	caret_pos = PREFIX_STRING.length();

	e_current_line = mgr->CreateEntity();
	e_current_line->Add<TextRenderable>(font);
	e_current_line->Get<TextRenderable>()->colour = text_colour;
	e_current_line->Get<TextRenderable>()->modifiers.scale = glm::vec2(FONT_SIZE);
	e_current_line->Get<TextRenderable>()->modifiers.valign = TextRenderable::Modifiers::V_TOP;
	e_current_line->Get<TextRenderable>()->SetMaxWidth(
			640.f - caret->Get<Transform>()->GetScaleX() - left_padding);
	e_current_line->Get<Transform>()->SetPosX(left_padding);
	e_current_line->Get<Transform>()->SetPosY(FONT_SIZE + bottom_padding - (FONT_SIZE * cumulative_lines));// + FONT_SIZE);
}

void Console::Draw(const GameTime &gametime)
{
	UpdateText();

	if(state != VISIBLE && switching == false)
		return;

	glm::mat4 model;
	float pos_y = state == VISIBLE ?
		0 : -Window::GetActive()->GetFramebufferHeight();

	if(switching == true &&
			transition_timer.ElapsedMilliseconds() < TRANSITION_TIME)
	{
		transition_timer += gametime.GetFrameTime();
		if(state == HIDDEN) {
			pos_y = Lerp(0.f, -(float)Window::GetActive()->GetFramebufferHeight(),
					transition_timer.ElapsedMilliseconds() / TRANSITION_TIME);
		}
		else {
			pos_y = Lerp(-(float)Window::GetActive()->GetFramebufferHeight(), 0.f,
					transition_timer.ElapsedMilliseconds() / TRANSITION_TIME);
		}
	}
	else {
		switching = false;
		transition_timer.SetTime(0);
	}

	pos_y = fmax(pos_y, -Game::Get()->window->GetFramebufferHeight());
	pos_y = fmin(pos_y, 0);

	glDisable(GL_DEPTH_TEST);
	Camera *other_camera = Camera::GetActive();
	Camera::SetActive(camera);

	fb_console->Bind();
	mgr->Manage(gametime);

	glClearColor(background_colour.r, background_colour.g, background_colour.b,
			background_colour.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	mgr->FlushDraw(gametime);
	int y_offset = 0;
	y_offset += (cumulative_lines - 1) * FONT_SIZE;
	y_offset += e_current_line->Get<TextRenderable>()->GetLineLengths().size() * FONT_SIZE;

	camera->MakeOrtho(0, Window::GetActive()->GetFramebufferWidth(),
			Window::GetActive()->GetFramebufferHeight() - y_offset, -y_offset,
			-1.f, 1.f);
	camera->pos.SetPosZ(1.f);
	camera->Post();
	DefaultRenderer::Get()->Flush(gametime);
	fb_console->Unbind();

	model = glm::translate(model, glm::vec3(0.f, pos_y - y_offset, 0.f));
	model = glm::scale(model, glm::vec3(
			Window::GetActive()->GetFramebufferWidth(),
			Window::GetActive()->GetFramebufferHeight(),
			1)
	);

	DefaultRenderer::Get()->Renderer2D->Add(fb_console->tex, model,
			glm::vec4(1.f), glm::vec4(0, 0, 1, -1), 0);
	DefaultRenderer::Get()->Flush(gametime);

	Camera::SetActive(other_camera);

	glEnable(GL_DEPTH_TEST);
}
