#include "camera.h"

GLint Camera::uni_block = -1;
Camera* Camera::active_camera = 0;

Camera::Camera()
{
	block.proj = glm::perspective(1.f, 720.f/540.f, 0.1f, 10000.f);
	glGenBuffers(1, &ubo);

	if(active_camera == NULL)
		active_camera = this;
}

void Camera::Post()
{
	glm::vec3 view_pos = pos.GetPosition();

	if(view_pos == glm::vec3(0.f))
		view_pos.y = 0.000001f;

	block.view = glm::lookAt(view_pos, lookat.GetPosition(),
			glm::vec3(0.f, 1.f, 0.f));

	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraBlock), &block,
			GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, uni_block, ubo);
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
