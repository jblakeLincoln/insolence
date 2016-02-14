#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/matrix.hpp>
#include "entity.h"
#include "movement_component.h"

struct Camera : Entity
{
	Camera();

	glm::mat4 mat_projection;
	glm::mat4 mat_view;
	MovementComponent pos;
	MovementComponent lookat;

	virtual void Update() {}
	void Draw() {}

	void UpdateRenderer(RenderManager *r);

	void PanX(float);
	void PanY(float);
	void PanZ(float);
	void Pan(const glm::vec3&);
};

#endif
