#include "transform.h"

void Transform::Construct()
{
	model_changed = false;
	scale = glm::mat4(1.f);
}

Transform::Transform()
{
	Construct();
}

Transform::Transform(const glm::vec3& pos)
{
	Construct();

	SetPos(pos);
}

/*****************************************************************************/
/** Position */
/*****************************************************************************/
const glm::vec3 Transform::GetPos() const
{
	return glm::vec3(position[3][0],
			position[3][1],
			position[3][2]);
}

const float Transform::GetPosX() const
{
	return position[3][0];
}

const float Transform::GetPosY() const
{
	return position[3][1];
}

const float Transform::GetPosZ() const
{
	return position[3][2];
}

const glm::vec2 Transform::GetPosXY() const
{
	return glm::vec2(position[3][0], position[3][1]);
}


void Transform::SetPos(const glm::vec3& pos)
{
	position[3][0] = pos[0];
	position[3][1] = pos[1];
	position[3][2] = pos[2];

	model_changed = true;
}

void Transform::SetPos(float x, float y, float z)
{
	position[3][0] = x;
	position[3][1] = y;
	position[3][2] = z;

	model_changed = true;
}

void Transform::SetPosX(float x)
{
	position[3][0] = x;
	model_changed = true;
}

void Transform::SetPosY(float y)
{
	position[3][1] = y;
	model_changed = true;
}

void Transform::SetPosZ(float z)
{
	position[3][2] = z;
	model_changed = true;
}

void Transform::SetPosXY(float x, float y)
{
	position[3][0] = x;
	position[3][1] = y;
	model_changed = true;
}

void Transform::SetPosXY(const glm::vec2& pos)
{
	position[3][0] = pos[0];
	position[3][1] = pos[1];
	model_changed = true;
}

/*****************************************************************************/
/** Orientation */
/*****************************************************************************/
const glm::quat& Transform::GetOrientation() const
{
	return orientation;
}

void Transform::SetOrientation(const glm::quat& q)
{
	orientation = q;
	model_changed = true;
}

/*****************************************************************************/
/** Scale */
/*****************************************************************************/
const glm::vec3 Transform::GetScale() const
{
	return glm::vec3(scale[0][0],
			scale[1][1],
			scale[2][2]);
}

const float Transform::GetScaleX() const
{
	return scale[0][0];
}

const float Transform::GetScaleY() const
{
	return scale[1][1];
}

const float Transform::GetScaleZ() const
{
	return scale[2][2];
}

const glm::vec2 Transform::GetScaleXY() const
{
	return glm::vec2(scale[0][0], scale[1][1]);
}

void Transform::SetScale(const glm::vec3& s)
{
	scale[0][0] = s[0];
	scale[1][1] = s[1];
	scale[2][2] = s[2];

	model_changed = true;
}

void Transform::SetScale(float x, float y, float z)
{
	scale[0][0] = x;
	scale[1][1] = y;
	scale[2][2] = z;

	model_changed = true;
}

void Transform::SetScaleX(float x)
{
	scale[0][0] = x;
	model_changed = true;
}

void Transform::SetScaleY(float y)
{
	scale[1][1] = y;
	model_changed = true;
}

void Transform::SetScaleZ(float z)
{
	scale[2][2] = z;
	model_changed = true;
}

void Transform::SetScaleXY(float x, float y)
{
	scale[0][0] = x;
	scale[1][1] = y;
	model_changed = true;
}

void Transform::Move(const glm::vec3 &pos)
{
	position[3][0] += pos[0];
	position[3][1] += pos[1];
	position[3][2] += pos[2];

	model_changed = true;
}

void Transform::Move(float x, float y, float z)
{
	position[3][0] += x;
	position[3][1] += y;
	position[3][2] += z;
}

void Transform::MoveX(float x)
{
	position[3][0] += x;
	model_changed = true;
}

void Transform::MoveY(float y)
{
	position[3][1] += y;
	model_changed = true;
}

void Transform::MoveZ(float z)
{
	position[3][2] += z;
	model_changed = true;
}

void Transform::MoveXY(float x, float y)
{
	position[3][0] += x;
	position[3][1] += y;

	model_changed = true;
}

void Transform::MoveXY(const glm::vec2 &pos)
{
	position[3][0] += pos[0];
	position[3][1] += pos[1];

	model_changed = true;
}

const glm::mat4& Transform::GetModelMatrix()
{
	if(model_changed == true)
	{
		model_matrix = position *
			glm::toMat4(orientation) *
			scale;
	}

	return model_matrix;
}

void Transform::Translate(const glm::vec3 &trans)
{
	position = glm::translate(position, trans);
	model_changed = true;
}

void Transform::Rotate(float theta, const glm::vec3 &axes)
{
	orientation = glm::rotate(orientation, theta, axes);
	model_changed = true;
}

void Transform::ResetOrientation()
{
	orientation = glm::quat();
	model_changed = true;
}
