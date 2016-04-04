#include "movement.h"

void Movement::Construct()
{
	model_changed = false;
	scale = glm::mat4(1.f);
}

Movement::Movement()
{
	Construct();
}

Movement::Movement(const glm::vec3& pos)
{
	Construct();

	SetPosition(pos);
}

const glm::vec3 Movement::GetPosition() const
{
	glm::vec3 ret_pos;

	for(int i = 0; i < 3; ++i)
		ret_pos[i] = position[3][i];

	return ret_pos;
}

const glm::vec3 Movement::GetScale() const
{
	glm::vec3 ret_scale;

	for(int i = 0; i < 3; ++i)
		ret_scale[i] = scale[i][i];

	return ret_scale;
}

const glm::mat4& Movement::GetModelMatrix()
{
	if(model_changed == true)
	{
		model_matrix = position *
			glm::toMat4(orientation) *
			scale;
	}

	return model_matrix;
}

void Movement::Translate(const glm::vec3 &trans)
{
	position = glm::translate(position, trans);
	model_changed = true;
}

void Movement::SetPosition(const glm::vec3& pos)
{
	for(int i = 0; i < 3; ++i)
		position[3][i] = pos[i];
	model_changed = true;
}

void Movement::SetOrientation(const glm::quat& q)
{
	orientation = q;
	model_changed = true;
}

void Movement::SetScale(const glm::vec3& s)
{
	for(int i = 0; i < 3; ++i)
		scale[i][i] = s[i];

	model_changed = true;
}

void Movement::Rotate(float theta, const glm::vec3 &axes)
{
	orientation = glm::rotate(orientation, theta, axes);
	model_changed = true;
}

void Movement::Scale(const glm::vec3 &s)
{
	for(int i = 0; i < 3; ++i)
		scale[i][i] += s[i];

	model_changed = true;
}

void Movement::Move(const glm::vec3 &pos)
{
	for(int i = 0; i < 3; ++i)
		position[3][i] += pos[i];

	model_changed = true;
}

void Movement::MoveX(float f)
{
	position[3].x += f;
	model_changed = true;
}

void Movement::MoveY(float f)
{
	position[3].y += f;
	model_changed = true;
}

void Movement::MoveZ(float f)
{
	position[3].z += f;
	model_changed = true;
}

void Movement::ResetOrientation()
{
	orientation = glm::quat();
	model_changed = true;
}

void Movement::ScaleX(float f)
{
	scale[0][0] = f;
	model_changed = true;
}

void Movement::ScaleY(float f)
{
	scale[1][1] = f;
	model_changed = true;
}

void Movement::ScaleZ(float f)
{
	scale[2][2] = f;
	model_changed = true;
}
