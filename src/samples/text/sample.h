#include <stdio.h>
#include <algorithm>
#include "../../game/game_world.h"

#include "../../render/render_manager_3d.h"
#include "../../render/render_manager_2d.h"
#include "../../component/camera.h"
#include "../../component/entity.h"
#include "../../component/font.h"
#include "../../component/material.h"

struct SampleText : BaseGameWorld
{
	SampleText() {}
	RenderManager2D *renderer_2d;
	Camera *camera;
	Font *f;

	void Initialise()
	{
		/* Init. */
		Texture::Init();
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		renderer_2d = new RenderManager2D();

		camera = new Camera();
		camera->pos.Translate(glm::vec3(0.f, 0.f, 560.f));
		//camera->pos.Rotate(10.f, glm::vec3(0.f, 1.f, 0.f));
		//camera->PanX(200);

		f = Font::Load("/usr/share/fonts/truetype/tlwg/Kinnari-Bold.ttf", 48);
	}

	void Update(const GameTime& gametime)
	{
	}

	void Draw()
	{
		renderer_2d->AddText(f, "MrotavatorW", glm::vec2(0, 0), glm::vec4(0.f,
					0.f, 0.f, 1.f), FontAlign::LEFT);
		renderer_2d->AddText(f, "WrotavatorW", glm::vec2(0, 36 * 1),
				glm::vec4(0.f, 0.f, 0.f, 1.f), FontAlign::CENTRE);
		renderer_2d->AddText(f, "WrotavatorM", glm::vec2(0, 36 * 2),
				glm::vec4(0.f, 0.f, 0.f, 1.f), FontAlign::RIGHT);

		camera->Post();
		renderer_2d->Flush();
	}

	void Unload()
	{

		delete f;
		delete camera;
		delete renderer_2d;
	}

};
