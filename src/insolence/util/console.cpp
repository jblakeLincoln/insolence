#include "console.h"

#include "math.h"
#include "../game/camera.h"
#include "../game/font.h"
#include "../render/framebuffer.h"
#include "../system/sprite_renderable_system.h"

const std::string Console::PREFIX_STRING = "$ ";

Console::Console()
{
	camera = new Camera(Window::GetActive(), Camera::ORTHO);
	mgr = new EntityManager();
	font = Font::Load("samples_assets/ProFontWindows.ttf", PIXEL_SIZE);
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
	caret->Get<SpriteRenderable>()->colour = glm::vec4(0.2f, 0.2f, 0.6f, 1.f);
	caret->Get<Transform>()->SetPosY(bottom_padding / 2.f);

	for(size_t i = 0; i < 40; ++i) {
		Entity *e = mgr->CreateEntity();
		e->Add<TextRenderable>(font);
		e->Get<TextRenderable>()->colour = glm::vec4(0.8f, 0.8f, 0.8f, 1.f);
		e->Get<TextRenderable>()->modifiers.scale = glm::vec2(FONT_SIZE);
		e->Get<TextRenderable>()->modifiers.valign = TextRenderable::Modifiers::V_BOTTOM;
		e->Get<Transform>()->SetPosX(left_padding);
		e->Get<Transform>()->SetPosY(i * FONT_SIZE + (bottom_padding));
		lines.push_back(e);
	}

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
	float caret_vis_pos = 0;

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
	}

	/* Go to line start. */
	if(Input::GetKey(JKEY_KEY_HOME) == JKEY_PRESS)
		caret_pos = PREFIX_STRING.length();
	/* Go to line end. */
	if(Input::GetKey(JKEY_KEY_END) == JKEY_PRESS)
		caret_pos = current_line.length();

	for(size_t i = 0; i < caret_pos && i < current_line.length(); ++i) {
		const Font::FontInfo &glyph = font->GetScaledGlyph(current_line[i]);
		caret_vis_pos += glyph.ax * FONT_SIZE;
	}

	caret->Get<Transform>()->SetPosX(caret_vis_pos + left_padding);
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

	lines[0]->Get<TextRenderable>()->text = current_line;

	if(modification == true) {
		lines_list.push_back(std::move(current_line));
		current_line = PREFIX_STRING;
		caret_pos = PREFIX_STRING.length();

		int count = 1;
		for(std::vector<std::string>::reverse_iterator i = lines_list.rbegin();
				i != lines_list.rend() && count < 40; ++i, ++count) {
			lines[count]->Get<TextRenderable>()->text = *i;
		}
	}

	UpdateCaret();
}

void Console::Draw(const GameTime &gametime)
{
	UpdateText();

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

	Camera *other_camera = Camera::GetActiveCamera();
	Camera::SetActiveCamera(camera);
	glDisable(GL_DEPTH_TEST);

	fb_console->Bind();
	mgr->Manage(gametime);

	glClearColor(0.0f, 0.f, 0.f, 0.7f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->MakeOrtho(0, Window::GetActive()->GetFramebufferWidth(),
			Window::GetActive()->GetFramebufferHeight(), 0, -1.f, 1.f);
	camera->pos.SetPosZ(1.f);
	camera->Post();
	mgr->FlushDraw(gametime);
	DefaultRenderer::Get()->Flush(gametime);
	fb_console->Unbind();

	model = glm::translate(model, glm::vec3(0.f, pos_y, 0.f));
	model = glm::scale(model, glm::vec3(
			Window::GetActive()->GetFramebufferWidth(),
			Window::GetActive()->GetFramebufferHeight(),
			1)
	);

	DefaultRenderer::Get()->Renderer2D->Add(fb_console->tex, model,
			glm::vec4(1.f), glm::vec4(0, 0, 1, -1), 0);
	DefaultRenderer::Get()->Flush(gametime);

	Camera::SetActiveCamera(other_camera);

	glEnable(GL_DEPTH_TEST);
}
