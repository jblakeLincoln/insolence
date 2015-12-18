#ifndef ENTITY_H
#define ENTITY_H

#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "renderable.h"

struct Renderable;
struct Entity
{
private:
	glm::mat4 model_matrix;
	bool model_changed;

	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;

public:

struct Entity
{
	Entity();
	~Entity() {}

	virtual void Update()=0;
	virtual void Draw()=0;
};

#endif
