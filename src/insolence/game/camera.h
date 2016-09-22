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
	enum Type {
		PERSPECTIVE,
		ORTHO
	};

	enum Coords {
		Y_UP,
		Y_DOWN
	};

	static void SetCoordinates(Coords c) {
		global_coords = c;
	}


	static Coords GetCoordinateSystem() {
		return global_coords;
	}

private:

	void SetCoordinateSystem(Coords c);
	static Coords global_coords;
	Coords local_coords;

	/*
	 * Ordered to send through to shader
	 */
	struct CameraBlock {
		glm::mat4 proj;
		glm::mat4 view;
	};

	struct PerspectiveSettings {
		float fov = 1.f;
		float aspect_ratio = 1.f;
		float z_near = 0.1f;
		float z_far = 1000.f;
	} perspective;

	struct OrthoSettings {
		float left = 0;
		float right = 0;
		float bottom = 0;
		float top = 0;
		float z_near = -1;
		float z_far = 1;
	} ortho;

	GLuint ubo;
	glm::vec3 upwards_vector;
	Coords coord_system;
	Type type;

	Window *window = NULL;
	int window_fb_width;
	int window_fb_height;

	static GLint uni_block;
	static Camera *active_camera;

	void Construct();

public:
	Camera();
	Camera(uint32_t width, uint32_t height);
	Camera(Window *w, Type t = Type::PERSPECTIVE);

	CameraBlock block;
	Transform pos;
	Transform lookat;

	static Camera* GetActiveCamera() {
		return active_camera;
	}

	static void SetActiveCamera(Camera* c) {
		active_camera = c;
	}

#ifdef INSOLENCE_WEBGL
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

#elif INSOLENCE_WEBGL
		uniform_dict[program].proj =
			glGetUniformLocation(program->GetID(), "mat_proj");
		uniform_dict[program].view =
			glGetUniformLocation(program->GetID(), "mat_view");
		uniform_dict[program].manually_posted = false;
#endif
	}

	void Post();

	void PanX(float);
	void PanY(float);
	void PanZ(float);
	void Pan(const glm::vec3&);

	void MakeOrtho(float l, float r, float btm, float top, float zn, float zf);
	void MakePerspective(float fov, float aspect, float znear, float zfar);

	void UpdatePerspective();
	void UpdateOrtho();

	void SetType(Type t);
};

#endif
