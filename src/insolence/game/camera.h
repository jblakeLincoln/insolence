#ifndef CAMERA_H
#define CAMERA_H

#include "../insolence_dll.h"

#include <GL/glew.h>
#include <glm/matrix.hpp>

#include "../component/transform.h"
#include "../render/shader_program.h"

struct Window;
struct INSOLENCE_API Camera
{
private:

	struct CameraBlock {
		glm::mat4 proj;
		glm::mat4 view;
	};

	Window *window = NULL;
	int window_fb_width;
	int window_fb_height;

	GLuint ubo;
	static GLint uni_block;
	static Camera *active_camera;

	void Construct();

public:
	Camera();
	Camera(uint32_t width, uint32_t height);
	Camera(Window*);

	CameraBlock block;
	Transform pos;
	Transform lookat;

	float fov;
	float aspect_ratio;
	float z_near;
	float z_far;

	static Camera* GetActiveCamera() {
		return active_camera;
	}

	static void SetActiveCamera(Camera* c) {
		active_camera = c;
	}

#ifdef INSOLENCE_OPENGL_ES
	struct CameraUniforms {
		int proj;
		int view;
		bool manually_posted;
	};
	static std::unordered_map<ShaderProgram*, CameraUniforms> uniform_dict;

	static void PostToShader_ES(ShaderProgram *shader_program) {
		if(shader_program == NULL || active_camera == NULL)
			return;

		glUseProgram(shader_program->GetID());

		int uni_proj = glGetUniformLocation(shader_program->GetID(),
				"mat_proj");
		int uni_view = glGetUniformLocation(shader_program->GetID(),
				"mat_view");

		glUniformMatrix4fv(uni_proj, 1, GL_FALSE,
				&active_camera->block.proj[0][0]);
		glUniformMatrix4fv(uni_view, 1, GL_FALSE,
				&active_camera->block.view[0][0]);

		uniform_dict[shader_program].manually_posted = true;
	}
#endif

	static void Setup(ShaderProgram *program)
	{
		if(program == NULL)
			return;

#ifdef INSOLENCE_OPENGL_DESKTOP
		uni_block = glGetUniformBlockIndex(program->GetID(), "Camera");

#elif INSOLENCE_OPENGL_ES
		uniform_dict[program].proj =
			glGetUniformLocation(program->GetID(), "mat_proj");
		uniform_dict[program].view =
			glGetUniformLocation(program->GetID(), "mat_view");
		uniform_dict[program].manually_posted = false;
#endif
	}

	virtual void Update() {}
	void Draw() {}

	void Post();

	void PanX(float);
	void PanY(float);
	void PanZ(float);
	void Pan(const glm::vec3&);
};

#endif
