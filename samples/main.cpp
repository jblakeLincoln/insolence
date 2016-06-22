#include <insolence/insolence.h>
#include <insolence/game/entity_manager.h>

#include <insolence/component/mesh_renderable.h>
#include <insolence/component/sprite_renderable.h>
#include <insolence/component/text_renderable.h>
#include <insolence/component/rigid_body.h>
//#include <insolence/physics/physics_manager.h>
#include <insolence/render/render_manager_2d.h>
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

struct A : Component {};
struct B : Component {};
struct C : Component {};
struct D : Component {};

template<> struct System<A> : SystemBase<A, 60> {};
template<> struct System<B> : SystemBase<B, 50> {};
template<> struct System<C> : SystemBase<C, 30> {};
template<> struct System<D> : SystemBase<D, 40> {};

struct Health : Component {
	int health = 0;
	Health(int i) { health = i; }
};

template<>
struct System<Health> : SystemBase<Health> {
};

struct Game1 : BaseGameWorld
{
	std::vector<Entity*> entities;

	Camera *camera;
	Mesh *mesh;
	Material *material;

	int counter = 0;
	float count = 100.f;

	std::vector<Entity*> texts;
	Font *font;

	Entity *centre;

	void Initialise()
	{
		mesh = Mesh::LoadFile("assets/crate.obj");
		material = new Material();
		material->diffuse =
			Texture::LoadColour(glm::vec4(1.f, 1.f, 1.f, 1.f));

		camera = new Camera(GetWindow());
		camera->pos.MoveZ(500.f);
		//camera->PanX(500);

		font = Font::Load(
				//"assets/joystix.ttf", 60);
				"assets/upheavtt.ttf", 20);
				//"/usr/share/fonts/truetype/tlwg/Kinnari.ttf", 20);
				//"/usr/share/fonts/truetype/droid/DroidSansMono.ttf", 80);
				//
		/*
		for(int i = 0; i < 3; ++i)
		{
			texts.push_back(CreateEntity());
			texts[i]->Add<TextRenderable>(font);
			texts[i]->Get<TextRenderable>()->Text("Hello");
			texts[i]->Get<TextRenderable>()->colour = glm::vec4(0.f, 0.f, 0.f, 1.f);
			texts[i]->Get<Transform>()->MoveY(-(i * 64));
		}


		texts[0]->Get<TextRenderable>()->align = FontAlign::LEFT;
		texts[1]->Get<TextRenderable>()->align = FontAlign::CENTRE;
		texts[2]->Get<TextRenderable>()->align = FontAlign::RIGHT;
		*/
		centre = CreateEntity();

		printf("** Created entity\n");
		CheckCentre();
		centre->Get<Transform>()->SetScale(glm::vec3(20.f));
		centre->Add<SpriteRenderable>();
		printf("** Added SpriteRenderable\n");
		CheckCentre();
		centre->Get<SpriteRenderable>()->texture = material->diffuse;
		centre->Add<Health>(20);
		printf("** Added Health\n");
		CheckCentre();
		centre->Remove<SpriteRenderable>();
		printf("** Removed SpriteRenderable\n");
		CheckCentre();
		centre->Remove<Health>();
		printf("Removed Health\n");
		CheckCentre();

		printf("Has again: %d\n", centre->Has<Health>());
		centre->Add<Health>(20);
		printf("Has again: %d\n", centre->Has<Health, Transform>());
		centre->Add<A>();
		centre->Add<B>();
		centre->Add<C>();
		centre->Add<D>();

		printf("Masky: %u\n", entity_manager->GetBitmask<A, B, C, D>());
		uint32_t bitmask_no = entity_manager->GetBitmask<D, MeshRenderable>();
		uint32_t bitmask_yes = entity_manager->GetBitmask<A, B, C, D>();

		printf("Should not have %u: %d\n", bitmask_no, centre->Has(bitmask_no));
		printf("Should have %u: %d\n", bitmask_yes, centre->Has(bitmask_yes));

		centre->Add<TextRenderable>(font)->Text("Hello World, here we are!");
		centre->Get<TextRenderable>()->colour = glm::vec4(0.9f, 0.6f, 0.1f, 1.f);
		centre->Get<TextRenderable>()->align = FontAlign::RIGHT;
		centre->Add<SpriteRenderable>()->colour = glm::vec4(0.f, 1.f, 0.f, 1.f);
	}

	void CheckCentre() {
		printf("Has Health: %d\n", centre->Has<Health>());
		printf("Has Transform: %d\n", centre->Has<Transform>());
		printf("Has SpriteRenderable: %d\n", centre->Has<SpriteRenderable>());
		printf("Has Health & Transform: %d\n", centre->Has<Health, Transform>());
		printf("Has all: %d\n\n", centre->Has<Health, Transform, SpriteRenderable>());
	}

	void Spawn(int i)
	{
		entities.push_back(CreateEntity());
		MeshRenderable * m = entities.back()->Add<MeshRenderable>(mesh, material,
				glm::vec4(0.3f, 0.6f, 0.1f, 1.f));

		m->colour.x = 1.f;
		entities.back()->Get<Transform>()->Move(
				glm::vec3(i * 1.2f, 1.f, 0.f));
		entities.back()->Get<Transform>()->SetScale(glm::vec3(20.f));
		//entities.back()->Add<RigidBody>();
		//entities.back()->Get<RigidBody>()->Make2D();
	}

	void Update(const GameTime& time)
	{
		if(Input::GetKey(JKEY_KEY_S) == JKEY_PRESS)
			Spawn(0);

		if(Input::GetKey(JKEY_KEY_D) == JKEY_PRESS && entities.size() > 0)
		{
			entities[0]->Destroy();
			entities.erase(entities.begin());
		}

		if (Input::GetPadButton(JPAD_BTN_Y) && entities.size() > 0)
			entities[0]->Get<Transform>()->MoveY(0.1f);
		if (Input::GetPadButton(JPAD_BTN_A) && entities.size() > 0)
			entities[0]->Get<Transform>()->MoveY(-0.1f);

		if(entities.size() > 0)
		{
			entities[0]->Get<Transform>()->Rotate(0.005f,
					glm::vec3(0.f, 1.f, 0.f));
			//entities[0]->Get<Transform>()->MoveX(0.1f * time.GetDeltaTime());

			if(++counter > count)
				counter = 0;

			/*
			for(int i = 0; i < count; ++i)
			{
				entities[0]->Destroy();
				entities.erase(entities.begin());
				//Spawn(counter);
			}
			*/
		}

		const TimeSpan &et = time.GetElapsedTime();
	}

	void Unload()
	{
		delete font;
		delete mesh;
		delete material;
		delete camera;
	}
};

int main()
{
	Game1 *game = new Game1();
	game->Run();

	delete game;

	return 0;
}


