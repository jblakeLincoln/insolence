#ifndef MOVEMENT_COMPONENT
#define MOVEMENT_COMPONENT

#include "../insolence_dll.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cstdio>

/* TODO Issue #20:	Make sure returned properties consider each other and
 *					any changes that could have occurred.
 */
struct INSOLENCE_API Transform {
private:
	void Construct();

	glm::mat4 model_matrix;
	bool model_changed;

	glm::mat4 position;
	glm::quat orientation;
	glm::mat4 scale;

public:
	Transform();
	Transform(const glm::vec3& pos);

	const glm::vec3 GetPos() const;
	const float GetPosX() const;
	const float GetPosY() const;
	const float GetPosZ() const;
	const glm::vec2 GetPosXY() const;

	void SetPos(const glm::vec3&);
	void SetPos(float, float, float);
	void SetPosX(float);
	void SetPosY(float);
	void SetPosZ(float);
	void SetPosXY(float, float);
	void SetPosXY(const glm::vec2&);

	const glm::quat& GetOrientation() const;
	void SetOrientation(const glm::quat&);

	const glm::vec3 GetScale() const;
	const float GetScaleX() const;
	const float GetScaleY() const;
	const float GetScaleZ() const;
	const glm::vec2 GetScaleXY() const;

	void SetScale(const glm::vec3&);
	void SetScale(float, float, float);
	void SetScaleX(float);
	void SetScaleY(float);
	void SetScaleZ(float);
	void SetScaleXY(float, float);

	void Move(const glm::vec3 &pos);
	void Move(float, float, float);
	void MoveX(float);
	void MoveY(float);
	void MoveZ(float);
	void MoveXY(float, float);
	void MoveXY(const glm::vec2&);

	/**
	 * Get the model matrix of position * rotation * scale.
	 * Updates the matrix before sending it back if any of those
	 * three things have been altered.
	 *
	 * \return	mat4 of model matrix.
	 */
	const glm::mat4& GetModelMatrix();

	/**
	 * Translate the position X, Y, Z relative to orientation.
	 *
	 * \param trans	X, Y, Z to translate by.
	 */
	void Translate(const glm::vec3 &trans);

	/**
	 * Rotate the orientation quaternion.
	 *
	 * \param theta	Degrees to rotate.
	 * \param axes	Axis of rotation.
	 */
	void Rotate(float theta, const glm::vec3 &axes);

	/**
	 * Reset orientation to 0 degrees on all axes.
	 */
	void ResetOrientation();
};

#endif
