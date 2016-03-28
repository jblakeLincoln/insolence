#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/matrix.hpp>

#include "entity.h"
#include "../component/movement.h"
#include "../render/shader_program.h"
#include "../render/render_manager.h"

struct Camera : Entity
{
private:
	struct CameraBlock {
		glm::mat4 proj;
		glm::mat4 view;
	};

	GLuint ubo;
	static GLint uni_block;

public:
	Camera();

	CameraBlock block;
	Movement pos;
	Movement lookat;

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
