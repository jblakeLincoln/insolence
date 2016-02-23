#include "movement_component.h"

MovementComponent::MovementComponent()
{
	model_changed = false;
	scale = glm::mat4(1.f);
}

const glm::vec3 MovementComponent::GetPosition() const
{
	glm::vec3 ret_pos;

	for(int i = 0; i < 3; ++i)
		ret_pos[i] = position[3][i];

	return ret_pos;
}

const glm::vec3 MovementComponent::GetScale() const
{
	glm::vec3 ret_scale;

	for(int i = 0; i < 3; ++i)
		ret_scale[i] = scale[i][i];

	return ret_scale;
}

const glm::mat4& MovementComponent::GetModelMatrix()
{
	if(model_changed == true)
	{
		model_matrix = position *
			glm::toMat4(orientation) *
			scale;
	}

	return model_matrix;
}

void MovementComponent::Translate(const glm::vec3 &trans)
{
	position = glm::translate(position, trans);
	model_changed = true;
}

void MovementComponent::SetPosition(const glm::vec3& pos)
{
	for(int i = 0; i < 3; ++i)
		position[3][i] = pos[i];
	model_changed = true;
}

void MovementComponent::SetOrientation(const glm::quat& q)
{
	orientation = q;
	model_changed = true;
}

void MovementComponent::SetScale(const glm::vec3& s)
{
	for(int i = 0; i < 3; ++i)
		scale[i][i] = s[i];

	model_changed = true;
}

void MovementComponent::Rotate(float theta, const glm::vec3 &axes)
{
	orientation = glm::rotate(orientation, theta, axes);
	model_changed = true;
}

void MovementComponent::Scale(const glm::vec3 &s)
{
	for(int i = 0; i < 3; ++i)
		scale[i][i] += s[i];

	model_changed = true;
}

void MovementComponent::Move(const glm::vec3 &pos)
{
	for(int i = 0; i < 3; ++i)
		position[3][i] += pos[i];

	model_changed = true;
}

void MovementComponent::MoveX(float f)
{
	position[3].x += f;
	model_changed = true;
}

void MovementComponent::MoveY(float f)
{
	position[3].y += f;
	model_changed = true;
}

void MovementComponent::MoveZ(float f)
{
	position[3].z += f;
	model_changed = true;
}

void MovementComponent::ResetOrientation()
{
	orientation = glm::quat();
	model_changed = true;
}

void MovementComponent::ScaleX(float f)
{
	scale[0][0] = f;
	model_changed = true;
}

void MovementComponent::ScaleY(float f)
{
	scale[1][1] = f;
	model_changed = true;
}

void MovementComponent::ScaleZ(float f)
{
	scale[2][2] = f;
	model_changed = true;
}
