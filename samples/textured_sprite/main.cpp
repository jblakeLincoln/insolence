#include <insolence/insolence.h>
#include <insolence/component/sprite_renderable.h>
#include <insolence/game/texture.h>

/**
 * A simple example of loading an image asset, assigning it as the sprite of
 * an entity, and controlling its animation properties.
 */

struct Game1 : BaseGameLoop
{
	Camera *camera;
	Texture *tex;
	Entity *sprite;

	void Initialise()
	{
		camera = new Camera(Game::Get()->window);
		camera->pos.MoveZ(2);

		tex = Texture::LoadFile("samples_assets/mega.png");

		sprite = Game::CreateEntity();
		/* Attach component that hooks into the 2D renderer. */
		sprite->Add<SpriteRenderable>(tex);
		/* Size of single frame on the spritesheet. */
		sprite->Get<SpriteRenderable>()->animation.size = glm::vec2(320, 240);
		sprite->Get<SpriteRenderable>()->animation.offset.y = 1.f;
		/* Number of columns of frames. */
		sprite->Get<SpriteRenderable>()->animation.cols = 7;
		/* Total number of frames to cycle through. */
		sprite->Get<SpriteRenderable>()->animation.frames = 48;
		/* Time per frame in milliseconds. */
		sprite->Get<SpriteRenderable>()->animation.frame_time = 40;

		/*
		 * Set the scale of the sprite to match the aspect ratio of a frame of
		 * the texture.
		 */
		sprite->Get<Transform>()->SetScaleXY(1.f + (1.f / 320.f), 1.f);
		/* Move our bottom-left anchored sprite to the centre of the screen. */
		sprite->Get<Transform>()->SetPosX(-0.5f + (1.f / 40.f));
		sprite->Get<Transform>()->SetPosY(-0.5f);
	}

	void Update(const GameTime& time)
	{
	}

	void Unload()
	{
		delete camera;
		delete tex;
	}
};

int main(int argc, char **argv)
{
	Game::Get()->loop = new Game1();
	Game::Get()->Run();

	return 0;
}
