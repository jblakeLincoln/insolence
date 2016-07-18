#include "camera.h"

#include "window.h"

GLint Camera::uni_block = -1;
Camera* Camera::active_camera = 0;
Camera::Coords Camera::global_coords = Y_UP;

#ifdef INSOLENCE_OPENGL_ES
std::unordered_map<ShaderProgram*, Camera::CameraUniforms> Camera::uniform_dict;
#endif

void Camera::Construct()
{
	glGenBuffers(1, &ubo);

	window = NULL;
	type = Type::PERSPECTIVE;
	local_coords = global_coords;
	UpdatePerspective();
	SetCoordinateSystem(Y_UP);

	if(active_camera == NULL)
		active_camera = this;
}

Camera::Camera()
{
	Construct();
}

Camera::Camera(uint32_t width, uint32_t height)
{
	Construct();
	perspective.aspect_ratio = (float)width / height;
}

Camera::Camera(Window *w, Type t)
{
	Construct();
	window = w;
	type = t;

	if(t == Type::PERSPECTIVE)
		UpdatePerspective();
	else if(t == Type::ORTHO)
		UpdateOrtho();
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
			perspective.aspect_ratio =
				(float)window_fb_width / window_fb_height;

		if(prev_w != window_fb_width || prev_h != window_fb_height ||
				local_coords != global_coords)
		{
			SetCoordinateSystem(global_coords);

			if(type == Type::PERSPECTIVE)
				UpdatePerspective();
			else if(type == Type::ORTHO)
			{
				ortho.right = window_fb_width;

				if(local_coords == Coords::Y_DOWN)
				{
					ortho.bottom = window_fb_height;
					ortho.top = 0;
				}
				else if(local_coords == Coords::Y_UP)
				{
					ortho.top = window_fb_height;
					ortho.bottom = 0;
				}

				UpdateOrtho();
			}

			glViewport(0, 0, window_fb_width, window_fb_height);
		}
	}

	glm::vec3 view_pos = pos.GetPos();

	if(view_pos == glm::vec3(0.f))
		view_pos.y = 0.000001f;
	if(type == Type::ORTHO)
		upwards_vector = glm::vec3(0, 1, 0);

	block.view = glm::lookAt(view_pos, lookat.GetPos(), upwards_vector);

#ifdef INSOLENCE_OPENGL_DESKTOP
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraBlock), &block,
			GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, uni_block, ubo);

#elif INSOLENCE_OPENGL_ES
	std::unordered_map<ShaderProgram*, CameraUniforms>::iterator it;
	for(it = uniform_dict.begin(); it != uniform_dict.end(); ++it)
	{
		CameraUniforms *unis = &it->second;
		if(unis->manually_posted == true)
		{
			unis->manually_posted = false;
			continue;
		}

		glUseProgram(it->first->GetID());
		glUniformMatrix4fv(unis->proj, 1, GL_FALSE,
				&block.proj[0][0]);
		glUniformMatrix4fv(unis->view, 1, GL_FALSE,
				&block.view[0][0]);
	}
#endif
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

void Camera::SetCoordinateSystem(Coords c) {
	local_coords = global_coords;
	switch(local_coords) {
		case Y_UP:
			upwards_vector = glm::vec3(0, 1, 0);
			break;
		case Y_DOWN:
			upwards_vector = glm::vec3(0, -1, 0);
			break;
	}
}

void Camera::MakeOrtho(float left, float right, float bottom, float top,
		float znear, float zfar)
{
	ortho.left      = left;
	ortho.right     = right;
	ortho.bottom    = bottom;
	ortho.top       = top;
	ortho.z_near    = znear;
	ortho.z_far     = zfar;

	type = Type::ORTHO;
	UpdateOrtho();
}

void Camera::UpdateOrtho()
{
	block.proj = glm::ortho(
			ortho.left,
			ortho.right,
			ortho.bottom,
			ortho.top,
			ortho.z_near,
			ortho.z_far);
}

void Camera::MakePerspective(float fov, float aspect, float znear, float zfar)
{
	perspective.fov           = fov;
	perspective.aspect_ratio  = aspect;
	perspective.z_near        = znear;
	perspective.z_far         = zfar;

	type = Type::PERSPECTIVE;
	UpdatePerspective();
}

void Camera::UpdatePerspective()
{
	block.proj = glm::perspective(
			perspective.fov,
			perspective.aspect_ratio,
			perspective.z_near,
			perspective.z_far);
}
