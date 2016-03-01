#include "camera.h"

Camera::Camera()
{
	mat_projection = glm::perspective(1.f, 720.f/540.f, 0.1f, 1000.f);
	//mat_view = glm::lookAt(move.GetPosition(), glm::vec3(0.f),
	//		glm::vec3(0.f, 1.f, 0.f));
}

void Camera::UpdateRenderer(RenderManager *r)
{
	glm::vec3 view_pos = pos.GetPosition();

	if(r->uni_projection < 0 || r->uni_view < 0)
		return;

	/* TODO Issue #17: System for smart checking and swapping. */
	glUseProgram(r->shader_program->GetID());

	if(view_pos == glm::vec3(0.f))
		view_pos.y = 0.000001f;

	mat_view = glm::lookAt(view_pos, lookat.GetPosition(),
			glm::vec3(0.f, 1.f, 0.f));

	glUniformMatrix4fv(r->uni_projection, 1, GL_FALSE,
			&mat_projection[0][0]);

	glUniformMatrix4fv(r->uni_view, 1, GL_FALSE,
			&mat_view[0][0]);
}

void Camera::PanX(float f)
{
	pos.MoveX(f);
	lookat.MoveX(f);
}

void Camera::PanY(float f)
{
	pos.MoveY(f);
	lookat.MoveY(f);
}

void Camera::PanZ(float f)
{
	pos.MoveZ(f);
	lookat.MoveZ(f);
}

void Camera::Pan(const glm::vec3& m)
{
	pos.Move(m);
	lookat.Move(m);
}
