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

	static void Setup(ShaderProgram *p)
	{
		uni_block = glGetUniformBlockIndex(p->GetID(),
				"Camera");
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
