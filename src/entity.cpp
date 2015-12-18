#include "entity.h"

Entity::Entity()
{
	model_changed = false;
	scale = glm::vec3(1.f);
}

const glm::mat4& Entity::GetModelMatrix()
{
	if(model_changed == true)
	{
		model_matrix = glm::translate(glm::mat4(1.f), position) *
			glm::toMat4(orientation) *
			glm::scale(glm::mat4(1.f), scale);
	}

	return model_matrix;
}

void Entity::Translate(const glm::vec3 &trans)
{
	GetModelMatrix();
	model_matrix = glm::translate(model_matrix, trans);

	for(int i = 0; i < 3; ++i)
		position[i] = model_matrix[3][i];

	model_changed = true;
}

void Entity::Rotate(float theta, const glm::vec3 &axes)
{
	orientation = glm::rotate(orientation, theta, axes);
	model_changed = true;
}

void Entity::Scale(const glm::vec3 &s)
{
	scale = s;
	model_changed = true;
}

void Entity::Move(const glm::vec3 &pos)
{
	position += pos;
	model_changed = true;
}

void Entity::MoveX(float f)
{
	position.x += f;
	model_changed = true;
}

void Entity::MoveY(float f)
{
	position.y += f;
	model_changed = true;
}

void Entity::MoveZ(float f)
{
	position.z += f;
	model_changed = true;
}

void Entity::ResetOrientation()
{
	orientation = glm::quat();
	model_changed = true;
}

void Entity::ScaleX(float f)
{
	scale.x = f;
	model_changed = true;
}

void Entity::ScaleY(float f)
{
	scale.y = f;
	model_changed = true;
}

void Entity::ScaleZ(float f)
{
	scale.z = f;
	model_changed = true;
}
