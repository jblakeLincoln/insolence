#include "sprite_renderable.h"

SpriteRenderable::SpriteRenderable(Entity *e)
	: Renderable(e)
{
}

SpriteRenderable::~SpriteRenderable()
{

}

void SpriteRenderable::Draw()
{
	if(tex != NULL)
		tex->Bind();

	Renderable::GetDefault2DRenderer()->Add(entity->GetModelMatrix(), glm::vec4(1.f));
}
