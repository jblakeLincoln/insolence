#ifndef MOVEMENT_COMPONENT
#define MOVEMENT_COMPONENT

#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct MovementComponent
{
private:
	glm::mat4 model_matrix;
	bool model_changed;

	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;

public:
	MovementComponent();

	/**
	 * \return	vec3 of position X, Y, Z.
	 */
	const glm::vec3& GetPosition() const { return position; }

	/**
	 * \return	Quaternion of orientation.
	 */
	const glm::quat& GetOrientation() const { return orientation; }

	/**
	 * \return	vec3 of scale X, Y, Z.
	 */
	const glm::vec3& GetScale() const { return scale; }

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
	 * Set the scale to the input X, Y, Z.
	 *
	 * \param s	Scale to set.
	 */
	void Scale(const glm::vec3 &s);

	/**
	 * Append to position X, Y, Z.
	 *
	 * \param pos	X, Y, Z to append to position.
	 */
	void Move(const glm::vec3 &pos);

	/**
	 * Move position along the X axis.
	 */
	void MoveX(float f);

	/**
	 * Move position along the Y axis.
	 */
	void MoveY(float f);

	/**
	 * Move position along the Z axis.
	 */
	void MoveZ(float f);

	/**
	 * Reset orientation to 0 degrees on all axes.
	 */
	void ResetOrientation();

	/**
	 * Scale the scale on the X axis to input.
	 */
	void ScaleX(float f);

	/**
	 * Set the scale on the Y axis to input.
	 */
	void ScaleY(float f);

	/**
	 * Set the scale on the Z axis to input.
	 */
	void ScaleZ(float f);
};

#endif
