#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/matrix.hpp>
#include "entity.h"
#include "movement_component.h"

struct Camera : Entity
{
	glm::mat4 mat_projection;
	glm::mat4 mat_view;
	MovementComponent move;

	Camera()
	{
		move.Move(glm::vec3(2.f, 0.f, 4.f));
		mat_projection = glm::perspective(1.f, 1.f, 0.1f, 1000.f);
		mat_view = glm::lookAt(move.GetPosition(), glm::vec3(0.f),
				glm::vec3(0.f, 1.f, 0.f));
	}

	virtual void Update() {}
	void Draw() {}

	void UpdateRenderer(RenderManager *r)
	{
		glm::vec3 view_pos = move.GetPosition();

		if(r->uni_projection < 0 || r->uni_view < 0)
			return;

		if(view_pos == glm::vec3(0.f))
			view_pos.y = 0.000001f;

		mat_view = glm::lookAt(view_pos, glm::vec3(0.f),
				glm::vec3(0.f, 1.f, 0.f));

		glUniformMatrix4fv(r->uni_projection, 1, GL_FALSE,
				&mat_projection[0][0]);

		glUniformMatrix4fv(r->uni_view, 1, GL_FALSE,
				&mat_view[0][0]);
	}
};

#endif
