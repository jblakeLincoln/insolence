#ifndef CAMERA_H
#define CAMERA_H

#include "../insolence_dll.h"

#include <GL/glew.h>
#include <glm/matrix.hpp>

#include "../component/movement.h"
#include "../render/shader_program.h"
#include "../render/render_manager.h"

struct INSOLENCE_API Camera
{
private:

	struct CameraBlock {
		glm::mat4 proj;
		glm::mat4 view;
	};

	GLuint ubo;
	static GLint uni_block;
	static Camera *active_camera;

public:
	Camera();

	CameraBlock block;
	Movement pos;
	Movement lookat;

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
