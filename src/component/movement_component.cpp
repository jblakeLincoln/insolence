#include "movement_component.h"

MovementComponent::MovementComponent()
{
	model_changed = false;
	scale = glm::vec3(1.f);
}

const glm::vec3& MovementComponent::GetPosition()
{
	const glm::mat4& m = GetModelMatrix();

	for(int i = 0; i < 3; ++i)
		position[i] = m[3][i];

	return position;
}

const glm::mat4& MovementComponent::GetModelMatrix()
{
	if(model_changed == true)
	{
		model_matrix = glm::translate(glm::mat4(1.f), position) *
			glm::toMat4(orientation) *
			glm::scale(glm::mat4(1.f), scale);
	}

	return model_matrix;
}

void MovementComponent::Translate(const glm::vec3 &trans)
{
	GetModelMatrix();
	model_matrix = glm::translate(model_matrix, trans);

	for(int i = 0; i < 3; ++i)
		position[i] = model_matrix[3][i];

	model_changed = true;
}

void MovementComponent::Rotate(float theta, const glm::vec3 &axes)
{
	orientation = glm::rotate(orientation, theta, axes);
	model_changed = true;
}

void MovementComponent::Scale(const glm::vec3 &s)
{
	scale = s;
	model_changed = true;
}

void MovementComponent::Move(const glm::vec3 &pos)
{
	position += pos;
	model_changed = true;
}

void MovementComponent::MoveX(float f)
{
	position.x += f;
	model_changed = true;
}

void MovementComponent::MoveY(float f)
{
	position.y += f;
	model_changed = true;
}

void MovementComponent::MoveZ(float f)
{
	position.z += f;
	model_changed = true;
}

void MovementComponent::ResetOrientation()
{
	orientation = glm::quat();
	model_changed = true;
}

void MovementComponent::ScaleX(float f)
{
	scale.x = f;
	model_changed = true;
}

void MovementComponent::ScaleY(float f)
{
	scale.y = f;
	model_changed = true;
}

void MovementComponent::ScaleZ(float f)
{
	scale.z = f;
	model_changed = true;
}
