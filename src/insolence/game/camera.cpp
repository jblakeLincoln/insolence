#include "camera.h"

GLint Camera::uni_block = -1;
Camera* Camera::active_camera = 0;

void Camera::Construct()
{
	glGenBuffers(1, &ubo);

	window = NULL;
	fov = 1.f;
	aspect_ratio = 1.f;
	z_near = 0.1f;
	z_far = 1000.f;

	if(active_camera == NULL)
		active_camera = this;
}

Camera::Camera()
{
	Construct();
	block.proj = glm::perspective(fov, aspect_ratio, z_near, z_far);
}

Camera::Camera(uint32_t width, uint32_t height)
{
	Construct();
	aspect_ratio = (float)width / height;
	block.proj = glm::perspective(fov, aspect_ratio, z_near, z_far);
}

Camera::Camera(Window *w)
{
	Construct();

	window = w;
	window_fb_width = window->GetFramebufferWidth();
	window_fb_height = window->GetFramebufferHeight();

	if(window_fb_width > 0 && window_fb_height > 0)
		aspect_ratio = (float)window_fb_width / window_fb_height;
	else
		aspect_ratio = 1;

	block.proj = glm::perspective(fov, aspect_ratio, z_near, z_far);
}

void Camera::Post()
{
	if(window != NULL)
	{
		int prev_w = window_fb_width;
		int prev_h = window_fb_height;

		window_fb_width = window->GetFramebufferWidth();
		window_fb_height= window->GetFramebufferHeight();

		if(window_fb_width > 0 && window_fb_height > 0)
			aspect_ratio = (float)window_fb_width / window_fb_height;

		if(prev_w != window_fb_width || prev_h != window_fb_height)
		{
			block.proj = glm::perspective(fov, aspect_ratio, z_near, z_far);
			glViewport(0, 0, window_fb_width, window_fb_height);
		}
	}

	glm::vec3 view_pos = pos.GetPos();

	if(view_pos == glm::vec3(0.f))
		view_pos.y = 0.000001f;

	block.view = glm::lookAt(view_pos, lookat.GetPos(),
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
